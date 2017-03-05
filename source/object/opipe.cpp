#include "c4d.h"
#include "c4d_symbols.h"
#include "opipe.h"
#include "ws_parameterhelp.h"
#include "main.h"

// be sure to use a unique ID obtained from www.plugincafe.com
const Int32 ID_PIPEOBJECT = 1027411;

class PipeObject : public ObjectData
{
	INSTANCEOF(PipeObject, ObjectData)

	private:
		Int32		GetProfileSplineID(Int32 cycleid);
		Bool		SetProfileParameters(SplineObject *op, Int32 lProfileType, Int32 lSplineType, Int32 lSplineInterpolation, Int32 SplineSub, Float SplineAngle, Float SplineMaxLen, Float rRad = 0.0, Int32 lSides = 0);

	public:
		virtual Bool Init(GeListNode *node);

		virtual BaseObject* GetVirtualObjects(BaseObject *op, HierarchyHelp *hh);
		virtual Bool Message(GeListNode *node, Int32 type, void *t_data);
		virtual Bool GetDEnabling(GeListNode *node, const DescID &id,const GeData &t_data,DESCFLAGS_ENABLE flags,const BaseContainer *itemdesc);

		static NodeData *Alloc(void) { return NewObjClear(PipeObject); }
};

// Initialize settings
Bool PipeObject::Init(GeListNode *node)
{
	BaseObject		*op		= (BaseObject*)node; if (!op) return false;
	BaseContainer *data = op->GetDataInstance(); if  (!data) return false;

	// Piper settings
	data->SetFloat(PIPEOBJECT_RAD_INNER,						45.0);
	data->SetFloat(PIPEOBJECT_RAD_OUTER,						50.0);
	data->SetBool(PIPEOBJECT_USEINNER,							true);
	data->SetBool(PIPEOBJECT_PROFILE,							PIPEOBJECT_PROFILE_CIRCLE);
	data->SetInt32(PIPEOBJECT_PROFILE_NSIDE_SIDES,	6);

	// Profile spline settings
	data->SetInt32(SPLINEOBJECT_TYPE,					SPLINEOBJECT_TYPE_LINEAR);
	data->SetInt32(SPLINEOBJECT_INTERPOLATION,	SPLINEOBJECT_INTERPOLATION_ADAPTIVE);
	data->SetInt32(SPLINEOBJECT_SUB,						8);
	data->SetFloat(SPLINEOBJECT_ANGLE,					Rad(10.0));
	data->SetFloat(SPLINEOBJECT_MAXIMUMLENGTH,	15.0);

	return true;
}

Bool PipeObject::Message(GeListNode *node, Int32 type, void *t_data)
{
	switch (type)
	{
		case MSG_DESCRIPTION_VALIDATE:
			{
//				BaseContainer *data = ((BaseObject*)node)->GetDataInstance();
//				*data, PIPEOBJECT_RAD_INNER, 0.0, data->GetFloat(PIPEOBJECT_RAD_OUTER));
			}
			break;

		case MSG_MENUPREPARE:
			((BaseObject*)node)->SetPhong(true,true,Rad(45.0));
			break;
	}

	return true;
}

// main routine: build virtual atom objects
BaseObject* PipeObject::GetVirtualObjects(BaseObject *op, HierarchyHelp *hh)
{
	BaseObject	*opChild = op->GetDown();
	if (!opChild || !opChild->IsInstanceOf(Ospline)) return nullptr;

	// Simple Caching
	Bool dirty =	op->CheckCache(hh) || op->IsDirty(DIRTYFLAGS_DATA) || op->IsDirty(DIRTYFLAGS_CACHE) ||
								opChild->IsDirty(DIRTYFLAGS_DATA) || opChild->IsDirty(DIRTYFLAGS_CACHE) || opChild->IsDirty(DIRTYFLAGS_MATRIX);
	if (!dirty)
		return op->GetCache(hh);

	// Container values: Piper settings
	BaseContainer	*bc = op->GetDataInstance(); if (!bc) return nullptr;
	Bool					bUseInner = bc->GetBool(PIPEOBJECT_USEINNER, false);
	Float					rInnerRad = bc->GetFloat(PIPEOBJECT_RAD_INNER, 45.0);
	Float					rOuterRad = bc->GetFloat(PIPEOBJECT_RAD_OUTER, 50.0);
	Int32					lSides = bc->GetInt32(PIPEOBJECT_PROFILE_NSIDE_SIDES, 6);
	Int32					lProfileType = GetProfileSplineID(bc->GetInt32(PIPEOBJECT_PROFILE, PIPEOBJECT_PROFILE_CIRCLE));

	// Container values: Spline settings
	Int32					lSplineType = bc->GetInt32(SPLINEOBJECT_TYPE, SPLINEOBJECT_TYPE_LINEAR);
	Int32					lSplineInterpolation = bc->GetInt32(SPLINEOBJECT_INTERPOLATION, SPLINEOBJECT_INTERPOLATION_ADAPTIVE);
	Int32					lSplineSub = bc->GetInt32(SPLINEOBJECT_SUB, 8);
	Float					rSplineAngle = bc->GetFloat(SPLINEOBJECT_ANGLE, Rad(10.0));
	Float					rSplineMaxLen = bc->GetFloat(SPLINEOBJECT_MAXIMUMLENGTH, 15.0);

	// New Objects
	BaseObject		*opPath = nullptr;
	BaseObject		*opSweep = nullptr;
	BaseObject		*opConnect = nullptr;
	BaseObject		*opProfile1 = nullptr;		// Outer profile
	BaseObject		*opProfile2 = nullptr;		// Inner profile

	// Create clone of path spline
	opPath = (BaseObject*)(opChild->GetClone(COPYFLAGS_0, nullptr));
	if (!opPath) goto Error;
	opChild->Touch();

	// Create SweepNURBS
	opSweep = BaseObject::Alloc(Osweep);
	if (!opSweep) goto Error;
	if (!SetAtomParameter(opSweep, CAP_TYPE, (Int32)CAP_TYPE_NGON)) goto Error;

	// Copy phong tag to SweepNURBS
	if (!op->CopyTagsTo(opSweep, true, false, false, nullptr)) goto Error;

	// Create outer profile
	opProfile1 = BaseObject::Alloc(lProfileType);
	if (!opProfile1) goto Error;
	if (!SetProfileParameters((SplineObject*)opProfile1, lProfileType, lSplineType, lSplineInterpolation, lSplineSub, rSplineAngle, rSplineMaxLen, rOuterRad, lSides)) goto Error;
	opProfile1->SetName(GeLoadString(IDS_PIPEOBJECT_PROFILE_OUTER));

	// If inner profile is desired
	if (bUseInner)
	{
		// Create Connect object
		opConnect = BaseObject::Alloc(Oconnector);
		if (!opConnect) goto Error;
		opConnect->SetName(GeLoadString(IDS_PIPEOBJECT_CONNECTOBJ));

		// Create inner profile
		opProfile2 = BaseObject::Alloc(lProfileType);
		if (!opProfile2) goto Error;
		if (!SetProfileParameters((SplineObject*)opProfile2, lProfileType, lSplineType, lSplineInterpolation, lSplineSub, rSplineAngle, rSplineMaxLen, rInnerRad, lSides)) goto Error;
		opProfile2->SetName(GeLoadString(IDS_PIPEOBJECT_PROFILE_INNER));

		// Build hierarchy
		opProfile2->InsertUnder(opConnect);
		opProfile1->InsertUnderLast(opConnect);
		opConnect->InsertUnder(opSweep);
		opPath->InsertUnderLast(opSweep);
	}
	else
	{
		// Build hierarchy
		opProfile1->InsertUnder(opSweep);
		opPath->InsertUnderLast(opSweep);
	}

	return opSweep;

Error:
	if (opPath) BaseObject::Free(opPath);
	if (opSweep) BaseObject::Free(opSweep);
	if (opConnect) BaseObject::Free(opConnect);
	if (opProfile1) BaseObject::Free(opProfile1);
	if (opProfile2) BaseObject::Free(opProfile2);

	return nullptr;
}

Bool PipeObject::GetDEnabling(GeListNode *node, const DescID &id,const GeData &t_data,DESCFLAGS_ENABLE flags,const BaseContainer *itemdesc)
{
	Int32 inter = 0;
	BaseContainer &data = *((BaseObject*)node)->GetDataInstance();

	switch (id[0].id)
	{
		// Piper attributes
		case PIPEOBJECT_RAD_INNER:
			return data.GetBool(PIPEOBJECT_USEINNER);

		case PIPEOBJECT_PROFILE_NSIDE_SIDES:
			return data.GetInt32(PIPEOBJECT_PROFILE) == PIPEOBJECT_PROFILE_NSIDE;

		// Spline attributes
		case SPLINEOBJECT_SUB:		
			inter=data.GetInt32(SPLINEOBJECT_INTERPOLATION);
			return inter==SPLINEOBJECT_INTERPOLATION_NATURAL || inter==SPLINEOBJECT_INTERPOLATION_UNIFORM;

		case SPLINEOBJECT_ANGLE:	
			inter = data.GetInt32(SPLINEOBJECT_INTERPOLATION);
			return inter==SPLINEOBJECT_INTERPOLATION_ADAPTIVE || inter==SPLINEOBJECT_INTERPOLATION_SUBDIV;

		case SPLINEOBJECT_MAXIMUMLENGTH:	
			return data.GetInt32(SPLINEOBJECT_INTERPOLATION)==SPLINEOBJECT_INTERPOLATION_SUBDIV;
	}

	return SUPER::GetDEnabling(node,id,t_data,flags,itemdesc);
}

Int32 PipeObject::GetProfileSplineID(Int32 cycleid)
{
	switch (cycleid)
	{
		default:
		case PIPEOBJECT_PROFILE_CIRCLE:
			return Osplinecircle;
		case PIPEOBJECT_PROFILE_NSIDE:
			return Osplinenside;
	}
}

Bool PipeObject::SetProfileParameters(SplineObject *op, Int32 lProfileType, Int32 lSplineType, Int32 lSplineInterpolation, Int32 lSplineSub, Float rSplineAngle, Float rSplineMaxLen, Float rRad, Int32 lSides)
{
	if (!op) return false;

	Bool result = true;

	result &= SetAtomParameter(op, SPLINEOBJECT_TYPE, lSplineType);
	result &= SetAtomParameter(op, SPLINEOBJECT_INTERPOLATION, lSplineInterpolation);
	result &= SetAtomParameter(op, SPLINEOBJECT_SUB, lSplineSub);
	result &= SetAtomParameter(op, SPLINEOBJECT_ANGLE, rSplineAngle);
	result &= SetAtomParameter(op, SPLINEOBJECT_MAXIMUMLENGTH, rSplineMaxLen);

	switch (lProfileType)
	{
		case Osplinecircle:
		{
			result &= SetAtomParameter(op, PRIM_CIRCLE_RADIUS, rRad);
		}
		case Osplinenside:
		{
			result &= SetAtomParameter(op, PRIM_NSIDE_RADIUS, rRad);
			result &= SetAtomParameter(op, PRIM_NSIDE_SIDES, lSides);
		}
	}

	return result;
}


Bool RegisterPipeObject(void)
{
	return RegisterObjectPlugin(ID_PIPEOBJECT, GeLoadString(IDS_PIPEOBJECT), OBJECT_GENERATOR|OBJECT_INPUT, PipeObject::Alloc, "opipe", AutoBitmap("opipe.tif"), 0);
}
