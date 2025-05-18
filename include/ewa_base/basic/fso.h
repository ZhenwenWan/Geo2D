#ifndef __H_EW_BASIC_FSO__
#define __H_EW_BASIC_FSO__

#include "ewa_base/basic/string.h"
#include "ewa_base/basic/stream.h"
#include "ewa_base/basic/file.h"
#include "ewa_base/util/regex.h"
#include "ewa_base/scripting/callable_data.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE IFileNameFilter : public Object
{
public:
	virtual bool test(const String& folder, const FileItem& name) const;

};


class DLLIMPEXP_EWA_BASE IFileNameFilterByName : public IFileNameFilter
{
public:
	Regex re;

	IFileNameFilterByName();
	IFileNameFilterByName(const String& pattern);

	virtual bool test(const String& folder, const FileItem& name) const;

	static bool assign_pattern(Regex& re, const String& pattern);
};


class DLLIMPEXP_EWA_BASE FSObject : public CallableData
{
public:

	virtual Stream Download(const String& fp)=0;
	virtual Stream Upload(const String& fp,int flag=FLAG_FILE_CR)=0;

	virtual bool FindFiles(const String& fp,arr_1t<FileItem>& items,const String& pattern)=0;

	virtual bool Mkdir(const String& fp,int flag=0)=0;
	virtual bool Rmdir(const String& fp,int flag)=0;
	virtual bool Remove(const String& fp_del)=0;
	virtual bool Rename(const String& fp_old,const String& fp_new,int flag)=0;

	virtual Stream Open(const String& fp,int flag=FLAG_FILE_RD);
	virtual bool DownloadToFile(const String& fp,const String& localfile,int flag=0);
	virtual bool DownloadToFolder(const String& fp,const String& localfile,int flag=0);
	virtual bool DownloadFolder(const String& fp,const String& localfolder,int flag=0);


	virtual bool DownloadToBuffer(const String& fp,StringBuffer& sb,int flag=FILE_TYPE_BINARY);
	virtual bool UploadFromFile(const String& localfile,const String& fp,int flag=0);
	virtual bool UploadFromBuffer(StringBuffer& sb,const String& fp,int flag);
	virtual bool UploadFromStream(Stream& sb, const String& fp, int flag);

	virtual int FileExists(const String& fp);
	arr_1t<FileItem> FindFilesEx(const String& fp,const String& pattern="*.*");

	void Filter(arr_1t<FileItem>& items,const String& pattern);

	static FSObject& current();

};

class DLLIMPEXP_EWA_BASE FSLocal : public FSObject
{
public:

	virtual Stream Download(const String& fp);
	virtual Stream Upload(const String& fp,int flag);

	virtual bool FindFiles(const String& fp,arr_1t<FileItem>& items,const String& pattern);

	virtual bool Mkdir(const String& fp,int flag);
	virtual bool Rmdir(const String& fp,int flag);
	virtual bool Rename(const String& fp_old,const String& fp_new,int flag);
	virtual bool Remove(const String& fp_del);

	virtual Stream Open(const String& fp, int flag = FLAG_FILE_RD);
	virtual bool DownloadToFile(const String& fp,const String& localfile,int flag);
	virtual bool DownloadToBuffer(const String& fp, StringBuffer& sb, int flag = FILE_TYPE_BINARY);
	virtual bool UploadFromFile(const String& localfile,const String& fp,int flag);
	virtual bool UploadFromBuffer(StringBuffer& sb,const String& fp,int flag);


	static String Map(const String& file);

	static FSLocal& current();

	DECLARE_OBJECT_INFO_CACHED(FSLocal,ObjectInfo);
};


class DLLIMPEXP_EWA_BASE IVirtualParam
{
public:
	String filename;
	String folder;
	String n, e, d;

};



class VirtualFileItem
{
public:
	VirtualFileItem();

	int type;
	BitFlags flags;
	int64_t lo;
	int64_t hi;

	void Serialize(Serializer& ar);
};

class FDVirtualFile : public ObjectData
{
public:

	FDVirtualFile();

	String filename;
	String n, e;
	indexer_map<String, VirtualFileItem> filemap;
	BitFlags flags;
	int version;

	bool Init(const String& file_, const String& n, const String& e);

	bool LoadReal(const String& vp, const String& fp, const IFileNameFilter& filter, arr_1t<FileItem>& files);
	bool LoadReal(const String& vp, const String& fp);
	bool LoadReal(const String& vp, const String& fp, const IFileNameFilter& filter);

	static bool GenerateVirtualFsFile(IVirtualParam& param, const IFileNameFilter&);

	void Serialize(SerializerHelper sh);
};

class DLLIMPEXP_EWA_BASE FSVirtual : public FSObject
{
public:

	FSVirtual();

	FSVirtual(const String& fp);

	Stream Download_virtual_only(const String& fp);
	bool FindFiles_virtual_only(const String& fp, arr_1t<FileItem>& items, const String& pattern);

	virtual Stream Download(const String& fp);
	virtual Stream Upload(const String& fp, int flag = FLAG_FILE_CR);

	virtual bool FindFiles(const String& fp, arr_1t<FileItem>& items, const String& pattern);

	static void InitVirtualPath(const bst_map<String,String>& fpmap);
	


	virtual bool Mkdir(const String& fp,int flag);
	virtual bool Rmdir(const String& fp, int flag);
	virtual bool Remove(const String& fp_del);
	virtual bool Rename(const String& fp_old, const String& fp_new, int flag);


	virtual int FileExists(const String& fp);

	static bool RegisterVirtualPath(const String& vp, const String& fp);
	static bool AttachFsFile(const String& vp, const String& fp, const String& n = "", const String& e = "");
	static bool GenerateVirtualFsFile(IVirtualParam& param, const IFileNameFilter& filter);
	static FSVirtual& get(const String& vp);

	bool MapToLocal(const String& file, String& fp);


protected:

	bool DoAttachFsFile(const String& fp, const String& n = "", const String& e = "");

	String folder;

	DataGrpT<FDVirtualFile> fsgrp;

	void DoSetFolder(const String& fp);

	DECLARE_OBJECT_INFO(FSVirtual, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE FSNetShared : public FSVirtual
{
public:

	FSNetShared();


	virtual Stream Download(const String& fp);
	virtual Stream Upload(const String& fp, int flag = FLAG_FILE_CR);

	virtual bool FindFiles(const String& fp, arr_1t<FileItem>& items, const String& pattern);

	virtual bool Mkdir(const String& fp, int flag);
	virtual bool Rmdir(const String& fp, int flag);
	virtual bool Remove(const String& fp_del);
	virtual bool Rename(const String& fp_old, const String& fp_new, int flag);

	virtual int FileExists(const String& fp);


	DECLARE_OBJECT_INFO(FSNetShared, ObjectInfo);
};


class DLLIMPEXP_EWA_BASE VirtualFilename
{
public:

	VirtualFilename(const String& fp_);
	VirtualFilename(const String& fp_,const String& cwd_);

	operator bool();
	FSObject* operator->();


	FSObject* fs;
	String fp;


	void reset(const String& fp_,const String& cwd_);

};


EW_LEAVE
#endif
