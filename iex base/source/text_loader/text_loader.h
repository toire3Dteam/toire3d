
#ifndef TEXT_LOADER_H
#define TEXT_LOADER_H

#include <stdio.h>
#include <cctype>
#include <assert.h>

class Text_loader
{
protected:
	char filename[256];
	FILE *file;
	fpos_t file_pointer;

	// スペース、改行などまで読み込み
	void Load_string(char *buf, unsigned int buf_size)
	{
		unsigned int buf_p(0);
		char letter;
		while (!feof(file) || buf_p > buf_size)
		{
			letter = fgetc(file);
			if (!isalnum(letter) && letter != '/' && letter != '.' && letter != '_' && letter != '{' && letter != '}')
			{
				if (buf_p == 0)
					continue;
				else
				{
					buf[buf_p] = '\0';
					return;
				}
			}
			buf[buf_p] = letter;
			buf_p++;
		}
		buf[buf_p] = '\0';
	}

	void Copy_string(char *dst, unsigned int dst_size, char *src)
	{
		unsigned int i = 0;
		for (; src[i] != '\0' && i < dst_size; i++)
		{
			dst[i] = src[i];
		}
		dst[i] = '\0';
	}
	void Add_string(char *dst, unsigned int dst_size, char *src1, char *src2)
	{
		assert(dst != src1 && dst != src2);

		unsigned int d(0);
		for (unsigned int i = 0; src1[i] != '\0' && d < dst_size; i++)
		{
			dst[d] = src1[i];
			d++;
		}
		for (unsigned int i = 0; src2[i] != '\0' && d < dst_size; i++)
		{
			dst[d] = src2[i];
			d++;
		}
		dst[d] = '\0';
	}

	bool Serch_string(char *str)
	{
		fgetpos(file, &file_pointer);

		unsigned int str_i(0);
		fpos_t fp(file_pointer);
		while (true)
		{
			if (fgetc(file) == str[str_i])
			{
				str_i++;
				if (str[str_i] == '\0')
					return true;
			}
			else
				str_i = 0;
			fp++;

			if (fp == file_pointer)
				return false;

			if (feof(file))
			{
				fp = 0;
				fsetpos(file, &fp);
				str_i = 0;
			}
		}

		return true;
	}
public:
	Text_loader(){}
	~Text_loader(){ Close(); }

	int Open(char *filename)
	{
		Copy_string(this->filename, 256, filename);
		fopen_s(&file, filename, "r");
		return ferror(file);
	}
	void Close(){ fclose(file); };
};

#endif // !TEXT_LOADER_H
