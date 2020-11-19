#pragma once

#include <string>

#include <Alime/base/base_define.h>
#include <Alime/base/compiler_specific.h>

namespace Alime::base
{
	///��ͳfopen��api��
	/// r��r+ Ҫ���ļ�������ڣ����򷵻ؾ��Ϊ��
	/// w,a,w+,a+���ᴴ���ļ�
	/// ��a/a+ģʽ�£� ÿ��д��ʱ������seek���ļ�β���� ��ʹ��ǰfseek��ǰ���ĳ��λ�á�
	/// a��a+���������ڣ�a+�ɶ�(��aΪ�����w����)
	/// ��C#��, �����˸���ģʽ�Ա�����ļ��Ƿ񱻴���
	enum class FileMode
	{
		Append,//��׷�ӷ�ʽ�������ļ����ļ��������򴴽���
		Create, //���´���һ���µ��ļ�������ļ��Ѵ��ڣ���ɾ�����ļ���Ȼ�󴴽�
		CreateNew,//ָ������ϵͳӦ����һ���µ��ļ����ļ��Ѵ������׳��쳣
		Open,//��һ�����е��ļ����ļ����������׳��쳣
		OpenOrCreate,//�������ļ����ļ��������򴴽����ļ���
		Truncate//�򿪲��ض����е��ļ��������ļ��ĳ�ʼ�������ڡ��ļ����������׳��쳣��
	};

	enum class FileAccess
	{
		Read,
		Write,
		ReadWrite
	};

	/// <summary>
	/// ���ڿؼ����̼��ļ�����Ȩ�ޣ���windows�£�������÷ǳ�ģ��
	/// </summary>
	enum class FileShare
	{
		None,
		Read,
		ReadWrite,
		Write,
		Delete,//û��
		Inheritable,//win32��֧��
	};

};


