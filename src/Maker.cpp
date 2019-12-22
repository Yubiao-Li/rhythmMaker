#include "Maker.h"
#include<iomanip>


Maker::Maker()
{

	fstream file;
	cout << "�뽫Ҫ���׵ĸ����ļ�����ڿ���������·��" << endl;
	string filename;
	getline(cin, filename);
	m_ac.load(filename);

	int index1 = filename.rfind('.');
	int index2 = filename.rfind('\\');
	m_spectrumfile = "..\\����\\" + filename.substr(index2 + 1, index1 - index2) + "txt";

	cout << "���������BPM:";
	cin >> m_BPM;
	m_timeperbeat = 60.0 / m_BPM;
}




Maker::~Maker()
{
	
}

void Maker::begintype()
{
	counter = 0;

	fstream file;
	file.open(m_spectrumfile, ios::binary | ios::out);

	file.write(reinterpret_cast<const char*>(&m_BPM), sizeof(m_BPM));

	//����ʱ
	for (int i = 3; i>0; i--)
	{
		cout << i << "���ʼ����" << endl;
		Sleep(1000);
	}
	
	//��������
	m_ac.play();

	//��ȡ����
	DWORD begin = ::GetTickCount(); //��ȡ���뼶��Ŀ
	double bgtime = begin / 1000.0; // bgΪ��
	while (1)
	{
		if (_kbhit())
		{
			char ch = _getch();
			if (ch != -32)
			{
				
				DWORD record = ::GetTickCount(); //��ȡ���뼶��Ŀ
				double recordtime = record / 1000.0; // ��λΪ��
				if (ch == 27)	// esc������
				{
					rhythm note;
					note.code = 0;
					note.token = m_ac.milliseconds()/1000 / m_timeperbeat;
					note.key = '@';
					file.write(reinterpret_cast<const char*>(&note), sizeof(note));
					break;
				}
				counter++;
				rhythm note;
				note.code = counter;
				note.key = ch;
				note.token= (recordtime - bgtime) / m_timeperbeat;
				switch (ch)
				{
				case 'H':
					cout << note.code << " �ڵ� " << note.token << " �İ��¡���" << endl;
					break;
				case 'P':
					cout << note.code << " �ڵ� " << note.token << " �İ��¡���" << endl;
					break;
				case 'K':
					cout << note.code << " �ڵ� " << note.token << " �İ��¡���" << endl;
					break;
				case 'M':
					cout << note.code << " �ڵ� " << note.token << " �İ��¡���" << endl;
					break;
				case 32:
					cout << note.code << " �ڵ� " << note.token << " �İ���space��" << endl;
					break;
				default:
					break;
				}
				file.write(reinterpret_cast<const char*>(&note), sizeof(note));
			}

		}
	
	}
	cout << "���׽���~" << endl;
	m_ac.stop();
	file.close();
}

void Maker::listening_test(double start_b)
{
	double had_pause = 0;	//����ͣʱ��
	rhythm data;
	fstream file;
	file.open(m_spectrumfile, ios::in | ios::binary);
	m_ac.play(start_b*m_timeperbeat*1000);
	
	
	DWORD begin = ::GetTickCount();
	double bgtime = begin / 1000.0; 
	file.seekg(sizeof(m_BPM));
	
	//˳�򲥷�������������
	while (file.read(reinterpret_cast<char*>(&data), sizeof(data)))
	{
		double pause_time;	//��ͣ��ʱ��
		double begin_time;	//���¿�ʼ��ʱ��
		

		DWORD record = ::GetTickCount(); //��ȡ���뼶��Ŀ
		double recordtime = record / 1000.0; 
		double wait_time = m_timeperbeat * data.token;
		double pass_time = recordtime - bgtime+start_b*m_timeperbeat-had_pause;

		
		while (pass_time < wait_time)
		{
			if (_kbhit())
			{
				char command;
				command = _getch();
				switch (command)
				{
				case 'z':
					m_ac.pause();
					pause_time =::GetTickCount()/1000.0;
					_getch();
					begin_time= ::GetTickCount() / 1000.0;
					had_pause += (begin_time - pause_time);
					m_ac.unpause();
					break;
				case 27:
					m_ac.stop();
					return;
					
				default:
					break;
				}
			}
			record = ::GetTickCount(); //��ȡ���뼶��Ŀ
			recordtime = record / 1000.0; 
			
			pass_time = recordtime - bgtime + start_b*m_timeperbeat-had_pause;
			
		}
		if (data.token >= start_b)
		{
			cout << data.code << setw(10) << data.token << setw(10) << data.key << endl;
		}
	}
	m_ac.stop();
	file.close();
}

void Maker::modify_time(int code)
{
	fstream file(m_spectrumfile, ios::out | ios::in|ios::binary);
	file.seekg(sizeof(m_BPM) + (code - 1) * sizeof(rhythm));
	cout << "�����ĺ�"<<endl;
	rhythm data;
	file.read(reinterpret_cast<char*>(&data), sizeof(data));
	cin >> data.token;
	file.seekp(sizeof(m_BPM) + (code - 1) * sizeof(rhythm));
	file.write(reinterpret_cast<char*>(&data), sizeof(data));
	file.close();
}

void Maker::modify_key(int code)
{
	fstream file(m_spectrumfile, ios::out | ios::in|ios::binary);
	file.seekg(sizeof(m_BPM) + (code - 1) * sizeof(rhythm));
	cout << "����key" << endl;
	rhythm data;
	file.read(reinterpret_cast<char*>(&data), sizeof(data));
	char ch = -32;
	while (ch == -32)
	{
		ch = _getch();
		data.key = ch;
	}
	cout << "press";
	cout << data.key<<endl;
	file.seekp(sizeof(m_BPM) + (code - 1) * sizeof(rhythm));
	file.write(reinterpret_cast<char*>(&data), sizeof(data));
	file.close();
}

void Maker::del(int code)
{
	fstream file(m_spectrumfile, ios::out | ios::in);
	rhythm data;
	file.seekg(sizeof(m_BPM) + (code - 1) * sizeof(rhythm));
	file.read(reinterpret_cast<char *>(&data), sizeof(data));
	data.code = 0;
	data.key = '@';
	file.seekp(sizeof(m_BPM) + (code - 1) * sizeof(rhythm));
	file.write(reinterpret_cast<char*>(&data), sizeof(data));
	file.close();
}