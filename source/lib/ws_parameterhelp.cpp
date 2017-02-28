#include "ws_parameterhelp.h"

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const Vector &value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, Float value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, Int32 value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const Matrix &value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const String &value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const Filename &value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const BaseTime &value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const BaseContainer &value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), GeData(value), descflags);
}

Bool SetAtomParameter(C4DAtom *dst, Int32 descid, GeData value, DESCFLAGS_SET descflags)
{
	if (!dst)
		return false;

	return dst->SetParameter(DescID((Int32)descid), value, descflags);
}
