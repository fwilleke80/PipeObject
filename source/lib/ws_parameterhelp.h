#ifndef __WS_PARAMETERHELP_H__
#define __WS_PARAMETERHELP_H__

#include "c4d.h"

// C4DAtom::SetParameter() convenience functions
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const Vector &value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, Float value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, Int32 value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const Matrix &value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const String &value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const Filename &value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const BaseTime &value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, const BaseContainer &value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);
Bool SetAtomParameter(C4DAtom *dst, Int32 descid, GeData value, DESCFLAGS_SET descflags = DESCFLAGS_SET_0);

#endif
