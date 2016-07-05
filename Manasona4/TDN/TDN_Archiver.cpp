#include	"TDNLIB.h"

/*****************/
/// �A�[�J�C�u
/*****************/
// �A�[�J�C�u�t�@�C�����쐬����
bool tdnArchiver::CreateArchiveFile(const char* pHeaderFilename, const char* pDataFilename)
{
	// �w�b�_�[���쐬
	headerOfs.open(pHeaderFilename, std::ios::binary);
	if (headerOfs.fail())
	{
		return false;
	}
	// �f�[�^���쐬
	dataOfs.open(pDataFilename, std::ios::binary);;
	if (dataOfs.fail())
	{
		return false;
	}

	ZeroMemory(nameID, sizeof(char[64]));
	wroteArchiveFileSize = 0;

	// �ő�w�b�_�[��
	HEAD_MAX = 0;

	return true;
}

// �f�[�^�t�@�C�����A�[�J�C�u�t�@�C���ɒǉ�����
bool tdnArchiver::AddData(const char* pDataFileName)
{
	// �w�b�_�[�t�@�C�����쐬�ς݂�
	if (!headerOfs.is_open())
	{
		return false;
	}

	// �f�[�^�t�@�C����ǂݍ���
	std::ifstream ifs; // input file stream
	ifs.open(pDataFileName, std::ios::binary);
	if (ifs.fail())
	{
		return false;
	}

	//------------------------------------------------
	// �f�[�^�t�@�C���̃T�C�Y���擾
	//------------------------------------------------
	// �t�@�C�������Ɉړ�
	ifs.seekg(0, std::fstream::end);
	// EOF(End of File)�܂�t�@�C�������̈ʒu���擾
	auto EOFPosition = ifs.tellg();

	// ��@�Ƃ��Ĉ�xClear��������Bfstream�̓����t���O�����Z�b�g�B
	ifs.clear();

	// �t�@�C���擪�Ɉړ�
	ifs.seekg(0, std::fstream::beg);
	// �t�@�C���擪�̈ʒu���擾
	auto BeginPosition = ifs.tellg();

	// �T�C�Y���Z�o����
	uint32_t dataFileSize = (uint32_t)(EOFPosition - BeginPosition);

	// �A�[�J�C�u�t�@�C���̍ő�T�C�Y�𒴂��Ă���Ȃ�G���[
	if (eMaxArchiveFileSize - wroteArchiveFileSize <
		sizeof(uint32_t)+											// �w�b�_�[�̍ő吔 
		sizeof(char[64]) + sizeof(uint32_t)+sizeof(uint32_t)+	// �w�b�_�[��
		dataFileSize +												// �f�[�^��
		sizeof(uint32_t)+sizeof(uint32_t))						// �_�~�[�w�b�_�[��
	{
		ifs.close();
		return false;
	}

	//------------------------------------------------
	// �������Ƀf�[�^�t�@�C���̓��e��W�J����
	//------------------------------------------------
	// �f�[�^�t�@�C������f�[�^��ǂݍ��ނ��߂̃o�C�g�z���new
	char* pData = new char[dataFileSize];
	// 0������
	memset(pData, 0, dataFileSize);

	// �f�[�^��ǂݍ���
	ifs.read(pData, dataFileSize);
	// ifs����Ă���
	ifs.close();


	//------------------------------------------------
	// �w�b�_�[���̐����J�E���g
	//------------------------------------------------
	HEAD_MAX++;// �ǉ������f�[�^�̐�

	//------------------------------------------------
	// �w�b�_�[���쐬
	//------------------------------------------------

	// ����0�ɏ�����
	ZeroMemory(nameID, sizeof(char[64]));
	memcpy(nameID, pDataFileName, sizeof(char[64]));// �����Ă������O���������ށI
	headerOfs.write((char*)&nameID, sizeof(char[64])); // �L�����^64���̏��𑗂�
	headerOfs.write((char*)&wroteArchiveFileSize, 4); // �V�[�N���邽�߂ɒǉ�
	headerOfs.write((char*)&dataFileSize, 4); // unsigned int(32bit�܂�4byte)����������

	//------------------------------------------------
	// �f�[�^���쐬
	//------------------------------------------------
	dataOfs.write(pData, dataFileSize);// �f�[�^�����f�[�^��`���܂�

	//------------------------------------------------
	// �i�ق�Ƃ́j�����ň��k������Í���������
	//------------------------------------------------

	//------------------------------------------------
	// �㏈��
	//------------------------------------------------
	delete[] pData;
	//dataID++;
	//wroteArchiveFileSize += sizeof(char[64]) + sizeof(uint32_t)+sizeof(uint32_t);
	wroteArchiveFileSize += dataFileSize;

	return true;
}

// �A�[�J�C�u�I������(�_�~�[�w�b�_�[�̒ǉ�)
bool tdnArchiver::CloseArchiveFile(const char* pArchiveFilename, const char* pHeaderFilename, const char* pDataFilename)
{
	if (!headerOfs.is_open())
	{
		return false;
	}

	//------------------------------------------------
	// �_�~�[�w�b�_�[�쐬
	//------------------------------------------------
	uint32_t dummy = -1;
	headerOfs.write((char*)&dummy, sizeof(char[64]));
	headerOfs.write((char*)&dummy, 4);// unsigned int �Ȃ̂Ł@-1�������Ɗђʂ���H
	headerOfs.write((char*)&dummy, 4);

	//------------------------------------------------
	// �t�@�C���N���[�Y
	//------------------------------------------------
	headerOfs.close();
	dataOfs.close();


	//--------------------------------------------------------
	// �Ō�Ɂ@�w�b�_�[���@�w�b�_�[���_�~�[�@�Ɓ@�f�[�^�@���������� 
	//--------------------------------------------------------
	// �w�b�_�[���쐬
	archiverOfs.open(pArchiveFilename, std::ios::binary);
	if (archiverOfs.fail())
	{
		return false;
	}


	/***************************************************/
	// �f�[�^�t�@�C����ǂݍ���
	std::ifstream ifs; // input file stream
	ifs.open(pHeaderFilename, std::ios::binary);
	if (ifs.fail())
	{
		return false;
	}
	//------------------------------------------------
	// �f�[�^�t�@�C���̃T�C�Y���擾
	//------------------------------------------------
	// �t�@�C�������Ɉړ�
	ifs.seekg(0, std::fstream::end);
	// EOF(End of File)�܂�t�@�C�������̈ʒu���擾
	auto EOFPosition = ifs.tellg();
	// ��@�Ƃ��Ĉ�xClear��������Bfstream�̓����t���O�����Z�b�g�B
	ifs.clear();
	// �t�@�C���擪�Ɉړ�
	ifs.seekg(0, std::fstream::beg);
	// �t�@�C���擪�̈ʒu���擾
	auto BeginPosition = ifs.tellg();
	// �T�C�Y���Z�o����
	uint32_t dataFileSize = (uint32_t)(EOFPosition - BeginPosition);
	//------------------------------------------------
	// �������Ƀf�[�^�t�@�C���̓��e��W�J����
	//------------------------------------------------
	// �f�[�^�t�@�C������f�[�^��ǂݍ��ނ��߂̃o�C�g�z���new
	char* pData = new char[dataFileSize];
	// 0������
	memset(pData, 0, dataFileSize);
	// �f�[�^��ǂݍ���
	ifs.read(pData, dataFileSize);
	// ifs����Ă���
	ifs.close();
	/*******************************************************/

	/*******************************************************/
	// �f�[�^�t�@�C����ǂݍ���
	std::ifstream ifs2; // input file stream
	ifs2.open(pDataFilename, std::ios::binary);
	if (ifs2.fail())
	{
		return false;
	}
	//------------------------------------------------
	// �f�[�^�t�@�C���̃T�C�Y���擾
	//------------------------------------------------
	// �t�@�C�������Ɉړ�
	ifs2.seekg(0, std::fstream::end);
	// EOF(End of File)�܂�t�@�C�������̈ʒu���擾
	auto EOFPosition2 = ifs2.tellg();
	// ��@�Ƃ��Ĉ�xClear��������Bfstream�̓����t���O�����Z�b�g�B
	ifs2.clear();
	// �t�@�C���擪�Ɉړ�
	ifs2.seekg(0, std::fstream::beg);
	// �t�@�C���擪�̈ʒu���擾
	auto BeginPosition2 = ifs2.tellg();
	// �T�C�Y���Z�o����
	uint32_t dataFileSize2 = (uint32_t)(EOFPosition2 - BeginPosition2);
	//------------------------------------------------
	// �������Ƀf�[�^�t�@�C���̓��e��W�J����
	//------------------------------------------------
	char* pData2 = new char[dataFileSize2];
	memset(pData2, 0, dataFileSize2);
	ifs2.read(pData2, dataFileSize2);
	ifs2.close();

	/*******************************************************/


	//------------------------------------------------
	// �w�b�_�[�ő吔�쐬
	//------------------------------------------------
	archiverOfs.write((char*)&HEAD_MAX, 4);

	//------------------------------------------------
	// �w�b�_�[���쐬
	//------------------------------------------------
	archiverOfs.write(pData, dataFileSize);

	//------------------------------------------------
	// �f�[�^���쐬
	//------------------------------------------------
	archiverOfs.write(pData2, dataFileSize2);

	// �t�@�C���N���[�Y
	archiverOfs.close();


	//------------------------------------------------
	// �㏈��
	//------------------------------------------------
	delete[] pData;
	delete[] pData2;


	return true;
}


/********************/
/// �A�[�J�C�u��
/********************/

//// �錾
//tdnUnArchiver* tdnUnArchiver::pInstance = nullptr;
//
//// ����
//tdnUnArchiver& tdnUnArchiver::GetInstance()
//{
//	if (!pInstance)
//	{
//		pInstance = new tdnUnArchiver;
//	}
//	return *pInstance;
//}
//
//// ���
//void tdnUnArchiver::Release()
//{
//	if (pInstance)
//	{
//		delete pInstance;
//		pInstance = nullptr;
//	}
//}

// �A�[�J�C�u�t�@�C������f�[�^���擾
char* tdnUnArchiver::OpenArchiveFile(const char* pArchiveFilename, const char* pName)
{
	// �A�[�J�C�u�t�@�C����ǂݍ���
	std::ifstream archiveIfs; // input file stream
	archiveIfs.open(pArchiveFilename, std::ios::binary);
	if (archiveIfs.fail())
	{
		return false;
	}

	// �t�@�C���擪�Ɉړ�
	//ifs.seekg(0, std::fstream::beg);

	// ���܂��擪�̃w�b�_�[�ő吔�����
	uint32_t HEAD_MAX = 0;
	archiveIfs.read((char*)&HEAD_MAX, 4);

	// �������o��
	for (int i = 0;; i++)
	{
		// �w�b�_�[���ǂݍ���
		ArchiveHeaderBlock headerBlock;
		size_t size = sizeof(ArchiveHeaderBlock);
		archiveIfs.read((char*)&headerBlock, sizeof(ArchiveHeaderBlock));

		// �w�啶���Ə���������ʂ����x��ID�̖��O�Ɠ����Ȃ�
		if (lstrcmpi(headerBlock.nameID, pName) == 0)// ��r�������ʂ������Ȃ�
		{
			// �f�[�^���ǂݍ���
			//ArchiveDataBlock dataBlock;

			//------------------------------------------------
			// �������Ƀf�[�^�t�@�C���̓��e��W�J����
			//------------------------------------------------
			// �f�[�^�t�@�C������f�[�^��ǂݍ��ނ��߂̃o�C�g�z���new
			// ��������new�����������͊O���Ńf���[�g���邵���Ȃ��Ƃ����n���B
			char* pData = new char[headerBlock.dataFileSize];
			// 0������
			//memset(pData, 0, headerBlock.dataFileSize);

			// �f�[�^����������ǂݍ��ޑO�ɂ��̃f�[�^�̂���Ƃ��܂ŃV�[�N
			// �w�b�_�[���̍ő�l�̒l +�@�J�n�ʒu����`�����񂾕��{�w�b�_�[�S�� + �_�~�[�w�b�_�[�@�܂ňړ��I
			archiveIfs.seekg(
				4 +												// �ŏ��̍ő�l
				(sizeof(ArchiveHeaderBlock)*HEAD_MAX) +			// �w�b�_�[�S��
				(sizeof(ArchiveHeaderBlock)) +					// �_�~�[�w�b�_�[ 
				headerBlock.wroteDataFileSize +					// �J�n�ʒu����`�����񂾕�
				std::fstream::beg);


			// �V�[�N�����f�[�^��������ǂݍ���
			archiveIfs.read(pData, headerBlock.dataFileSize);// �t�@�C���T�C�Y���ǂݍ���
			// ����pData�Ƀt�@�C���̏�񂪋l�܂�܂����B

			// ifs����Ă���
			archiveIfs.close();

			return pData;
		}

		// �w�b�_�[����-1(�_�~�[)�Ȃ�Γǂݍ��ݏI��
		if (headerBlock.dataFileSize == -1)
		{
			// ifs����Ă���
			archiveIfs.close();
			return false;
		}

	}

}


// �܂��A�[�J�C�u�t�@�C����ǂݍ���
int tdnUnArchiver::GetSize(const char* pArchiveFilename, const char* pName)
{

	// �A�[�J�C�u�t�@�C����ǂݍ���
	std::ifstream archiveIfs; // input file stream
	archiveIfs.open(pArchiveFilename, std::ios::binary);
	if (archiveIfs.fail())
	{
		return false;
	}

	// �t�@�C���擪�Ɉړ�
	//ifs.seekg(0, std::fstream::beg);

	// ���܂��擪�̃w�b�_�[�ő吔�����
	uint32_t HEAD_MAX = 0;
	archiveIfs.read((char*)&HEAD_MAX, 4);

	// �������o��
	for (int i = 0;; i++)
	{
		// �w�b�_�[���ǂݍ���
		ArchiveHeaderBlock headerBlock;
		size_t size = sizeof(ArchiveHeaderBlock);
		archiveIfs.read((char*)&headerBlock, sizeof(ArchiveHeaderBlock));

		// �w�啶���Ə���������ʂ����x��ID�̖��O�Ɠ����Ȃ�
		if (lstrcmpi(headerBlock.nameID, pName) == 0)// ��r�������ʂ������Ȃ�
		{
			// ifs����Ă���
			archiveIfs.close();
			return headerBlock.dataFileSize;
		}

		// �w�b�_�[����-1(�_�~�[)�Ȃ�Γǂݍ��ݏI��
		if (headerBlock.dataFileSize == -1)
		{
			// ifs����Ă���
			archiveIfs.close();
			return false;
		}

	}


}

// �A�[�J�C�u���J���{�������T�C�Y���擾
char* tdnUnArchiver::OpenArchiveFile(const char* pArchiveFilename, const char* pName, int& outMemorySize)
{
	// �������T�C�Y��������
	outMemorySize = 0;

	// �A�[�J�C�u�t�@�C����ǂݍ���
	std::ifstream archiveIfs; // input file stream
	archiveIfs.open(pArchiveFilename, std::ios::binary);
	if (archiveIfs.fail())
	{
		return false;
	}

	// �t�@�C���擪�Ɉړ�
	//ifs.seekg(0, std::fstream::beg);

	// ���܂��擪�̃w�b�_�[�ő吔�����
	uint32_t HEAD_MAX = 0;
	archiveIfs.read((char*)&HEAD_MAX, 4);

	// �������o��
	for (int i = 0;; i++)
	{
		// �w�b�_�[���ǂݍ���
		ArchiveHeaderBlock headerBlock;
		size_t size = sizeof(ArchiveHeaderBlock);
		archiveIfs.read((char*)&headerBlock, sizeof(ArchiveHeaderBlock));

		// �w�啶���Ə���������ʂ����x��ID�̖��O�Ɠ����Ȃ�
		if (lstrcmpi(headerBlock.nameID, pName) == 0)// ��r�������ʂ������Ȃ�
		{
			// �f�[�^���ǂݍ���
			//ArchiveDataBlock dataBlock;

			//------------------------------------------------
			// �������Ƀf�[�^�t�@�C���̓��e��W�J����
			//------------------------------------------------
			// �f�[�^�t�@�C������f�[�^��ǂݍ��ނ��߂̃o�C�g�z���new
			char* pData = new char[headerBlock.dataFileSize];
			// 0������
			//memset(pData, 0, headerBlock.dataFileSize);

			// �f�[�^����������ǂݍ��ޑO�ɂ��̃f�[�^�̂���Ƃ��܂ŃV�[�N
			// �w�b�_�[���̍ő�l�̒l +�@�J�n�ʒu����`�����񂾕��{�w�b�_�[�S�� + �_�~�[�w�b�_�[�@�܂ňړ��I
			archiveIfs.seekg(
				4 +												// �ŏ��̍ő�l
				(sizeof(ArchiveHeaderBlock)*HEAD_MAX) +			// �w�b�_�[�S��
				(sizeof(ArchiveHeaderBlock)) +					// �_�~�[�w�b�_�[ 
				headerBlock.wroteDataFileSize +					// �J�n�ʒu����`�����񂾕�
				std::fstream::beg);


			// �V�[�N�����f�[�^��������ǂݍ���
			archiveIfs.read(pData, headerBlock.dataFileSize);// �t�@�C���T�C�Y���ǂݍ���
			// ����pData�Ƀt�@�C���̏�񂪋l�܂�܂����B

			// ifs����Ă���
			archiveIfs.close();


			outMemorySize = headerBlock.dataFileSize;
			return pData;
		}

		// �w�b�_�[����-1(�_�~�[)�Ȃ�Γǂݍ��ݏI��
		if (headerBlock.dataFileSize == -1)
		{
			// ifs����Ă���
			archiveIfs.close();
			return false;
		}

	}

}