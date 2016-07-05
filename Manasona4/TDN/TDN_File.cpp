
#include	"TDNLIB.h"
#include<iostream>

// �t�H���_����邽�߂ɕK�v�ȃw�b�_
#include <direct.h>

// c++11�ɂĒǉ����ꂽ�t�@�C���֌W�֗̕��ȃ��C�u����
#include <filesystem> // std::tr2::sys::path etc.
namespace sys = std::tr2::sys;

// �ÓI�����o�ϐ��̐錾
char tdnFile::strFile[256] = "";		// �_�C�A���O�Ƃ��ł̃p�X

//*****************************************************
//		�t�@�C���֌W�ŕ֗��ɂȂ肻���Ȃ̂��܂Ƃ߂�N���X
//*****************************************************

int tdnFile::CreateFolder(char *path)
{
	// �f�B���N�g���쐬&���ʂ�Ԃ�(-1�Ȃ玸�s�A0�Ȃ琬��)
	return _mkdir(path);
}

void tdnFile::EnumDirectory(char *path, DirectoryInfo *out, bool bExt)
{
	sys::path p(path); // �񋓂̋N�_

	// for_each�ƃ����_�ɂ�郋�[�v�ł́A�X�R�[�v�O�̕ϐ��ɃA�N�Z�X�ł��Ȃ��̂ŕs�̗p
	//std::for_each(sys::directory_iterator(p), sys::directory_iterator(),[](const sys::path& p)
	//{
	//	if (sys::is_regular_file(p)) { // �t�@�C���Ȃ�...
	//		std::cout << "file: " << p.filename() << std::endl;
	//	}
	//	else if (sys::is_directory(p)) { // �f�B���N�g���Ȃ�...
	//		std::cout << "dir.: " << p.string() << std::endl;
	//	}
	//});

	// for���ŉ񂵂ė񋓂���
	for (std::tr2::sys::directory_iterator it(p), end; it != end; ++it)
	{
		// �t�@�C����������
		if (sys::is_regular_file(it->path()))
		{
			// �t�@�C�����̔z��Ɋi�[
			out->FileNames.push_back((bExt) ? it->path().filename() : it->path().stem());	// �O�����Z�q�Ŋg���q�t���������łȂ����ŕ����Ă���
		}

		// �t�H���_�[��������
		else if (sys::is_directory(it->path()))
		{
			// �t�H���_���̔z��Ɋi�[
			out->FolderNames.push_back(it->path().string());
		}
	}
}

std::string tdnFile::GetFileExtention(char *path)
{
	sys::path p(path);

	// �g���q��Ԃ�
	return p.extension();
}

std::string tdnFile::GetFileName(char *path, bool bExt)
{
	sys::path p(path);

	// true�Ȃ�g���q�t���Afalse�Ȃ�g���q�Ȃ��̃t�@�C������Ԃ�
	return (bExt) ? p.filename() : p.stem();
}

std::string tdnFile::OpenFileDialog(char *filter)
{
	LPCTSTR str_filter = TEXT(filter);

	// �������������d�v�Ȃ���
	// �_�C�A���O���J���֐�(GetOpenFileName)���g�������ifstream�֘A�̏������s���Ƃ���Ȃ����s���Ă��܂����Ƃ������A
	// ���ׂ����ʁAGetCurrentDirectry�ƃ_�C�A���O���SetCurrentDirectry���g���Ɩ����������ꂽ
	// ���܂�Ɋ����������̂�URL�\��http://stackoverflow.com/questions/20158334/get-open-filename-is-killing-ifstream
	char buffer[255];
	GetCurrentDirectoryA(255, buffer);

	OPENFILENAME of;
	memset(&of, 0, sizeof(OPENFILENAME));
	of.lStructSize = sizeof(OPENFILENAME);
	// �_�C�A���O�{�b�N�X�����L����E�B���h�E�ւ̃n���h��(�J���Ă�Œ��ɂ��̃E�B���h�E���������Ȃ��悤�ɂ���)
	of.hwndOwner = tdnSystem::GetWindow();
	of.lpstrFilter = TEXT(str_filter);
	// �t�@�C�������i�[�����o�b�t�@�̃A�h���X
	of.lpstrFile = (LPTSTR)strFile;
	// lpstrFile�����o�Ŏw�肳���o�b�t�@�̃T�C�Y
	of.nMaxFile = MAX_PATH;
	// �_�C�A���O�{�b�N�X�̏������Ɏg�p�����r�b�g�t���O
	// OFN_PATHMUSTEXIST:���[�U�[���L���ȃp�X
	//  ����уt�@�C�����݂̂���͂ł���悤�Ɏw��
	of.Flags = OFN_PATHMUSTEXIST;
	// �f�t�H���g�̊g���q���i�[�����o�b�t�@�̃A�h���X
	of.lpstrDefExt = TEXT("txt");
	// �R�����_�C�A���O�̕\��
	if (GetOpenFileName(&of))	// FALSE���Ԃ�����L�����Z���������ꂽ
	{
		// ������
		SetCurrentDirectory(buffer);

		// strFile�Ƀ_�C�A���O�őI�񂾃p�X���i�[����Ă���
		return std::string(strFile);
	}

	// �t�@�C���I�[�v���Ɏ��s�����̂ŁA��̕�����
	return std::string("");
}

std::string tdnFile::SaveFileDialog(char *filter)
{
	LPCTSTR str_filter = TEXT(filter);

	char buffer[255];
	GetCurrentDirectoryA(255, buffer);

	OPENFILENAME of;
	memset(&of, 0, sizeof(OPENFILENAME));
	of.lStructSize = sizeof(OPENFILENAME);
	// �_�C�A���O�{�b�N�X�����L����E�B���h�E�ւ̃n���h��(�J���Ă�Œ��ɂ��̃E�B���h�E���������Ȃ��悤�ɂ���)
	of.hwndOwner = tdnSystem::GetWindow();
	of.lpstrFilter = TEXT(str_filter);
	// �t�@�C�������i�[�����o�b�t�@�̃A�h���X
	of.lpstrFile = (LPTSTR)strFile;
	// lpstrFile�����o�Ŏw�肳���o�b�t�@�̃T�C�Y
	of.nMaxFile = MAX_PATH;
	// �_�C�A���O�{�b�N�X�̏������Ɏg�p�����r�b�g�t���O
	// OFN_PATHMUSTEXIST:���[�U�[���L���ȃp�X
	//  ����уt�@�C�����݂̂���͂ł���悤�Ɏw��
	of.Flags = OFN_PATHMUSTEXIST;
	// �f�t�H���g�̊g���q���i�[�����o�b�t�@�̃A�h���X
	of.lpstrDefExt = TEXT("txt");
	// �R�����_�C�A���O�̕\��
	if (GetSaveFileName(&of))	// FALSE���Ԃ�����L�����Z���������ꂽ
	{
		SetCurrentDirectory(buffer);

		// strFile�Ƀ_�C�A���O�őI�񂾃p�X���i�[����Ă���
		return std::string(strFile);
	}

	// �t�@�C���I�[�v���Ɏ��s�����̂ŁA��̕�����
	return std::string("");
}