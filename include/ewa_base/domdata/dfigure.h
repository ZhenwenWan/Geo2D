
#ifndef __H_EW_DOMDATA_DFIGURE__
#define __H_EW_DOMDATA_DFIGURE__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/dcoord.h"
#include "ewa_base/domdata/dattribute.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE DFigure : public DObjectBox
{
public:

	typedef DObjectBox basetype;

	DFigure();

	DataPtrT<DObject> m_pItem;
	arr_1t<DataPtrT<DObject> > m_aItems;
	DataPtrT<DAttributeManager> m_pAttributeManager;

	int __setindex(Executor& ks, const String& si);
	int __getindex(Executor& ks, const String& si);
	
	bool DoGetChildren(XCtxChildrenState&);
	bool DoAppendChild(DataPtrT<DObject> p);

	void Serialize(SerializerHelper sh);

	void DoTransferData(TableSerializer& ar);

	void GetIcon(XCtxGetImage& ctx) const;


	class FigureStateItem
	{
	public:
		DataPtrT<DObject> pobj;
		DataPtrT<DCoord> pcoord;
	};

	class FigureState : public FigureStateItem
	{
	public:
		LitePtrT<DObject> pfigure;
		DataPtrT<DFigData> pdata;
		arr_1t<FigureStateItem> states;
		BitFlags flags;
	}fs;


	DECLARE_DOMDATA_INFO(DFigure, DObjectInfo);
};


EW_LEAVE
#endif
