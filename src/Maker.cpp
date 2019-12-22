#include "Maker.h"
#include<iomanip>


Maker::Maker()
{

	fstream file;
	cout << "请将要打谱的歌曲文件拖入黑框框或者输入路径" << endl;
	string filename;
	getline(cin, filename);
	m_ac.load(filename);

	int index1 = filename.rfind('.');
	int index2 = filename.rfind('\\');
	m_spectrumfile = "..\\谱面\\" + filename.substr(index2 + 1, index1 - index2) + "txt";

	cout << "输入歌曲的BPM:";
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

	//倒计时
	for (int i = 3; i>0; i--)
	{
		cout << i << "秒后开始打谱" << endl;
		Sleep(1000);
	}
	
	//播放音乐
	m_ac.play();

	//读取键盘
	DWORD begin = ::GetTickCount(); //获取毫秒级数目
	double bgtime = begin / 1000.0; // bg为秒
	while (1)
	{
		if (_kbhit())
		{
			char ch = _getch();
			if (ch != -32)
			{
				
				DWORD record = ::GetTickCount(); //获取毫秒级数目
				double recordtime = record / 1000.0; // 单位为秒
				if (ch == 27)	// esc键结束
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
					cout << note.code << " 在第 " << note.token << " 拍按下↑键" << endl;
					break;
				case 'P':
					cout << note.code << " 在第 " << note.token << " 拍按下↓键" << endl;
					break;
				case 'K':
					cout << note.code << " 在第 " << note.token << " 拍按下←键" << endl;
					break;
				case 'M':
					cout << note.code << " 在第 " << note.token << " 拍按下→键" << endl;
					break;
				case 32:
					cout << note.code << " 在第 " << note.token << " 拍按下space键" << endl;
					break;
				default:
					break;
				}
				file.write(reinterpret_cast<const char*>(&note), sizeof(note));
			}

		}
	
	}
	cout << "打谱结束~" << endl;
	m_ac.stop();
	file.close();
}

void Maker::listening_test(double start_b)
{
	double had_pause = 0;	//总暂停时间
	rhythm data;
	fstream file;
	file.open(m_spectrumfile, ios::in | ios::binary);
	m_ac.play(start_b*m_timeperbeat*1000);
	
	
	DWORD begin = ::GetTickCount();
	double bgtime = begin / 1000.0; 
	file.seekg(sizeof(m_BPM));
	
	//顺序播放谱面所有内容
	while (file.read(reinterpret_cast<char*>(&data), sizeof(data)))
	{
		double pause_time;	//暂停的时刻
		double begin_time;	//重新开始的时刻
		

		DWORD record = ::GetTickCount(); //获取毫秒级数目
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
			record = ::GetTickCount(); //获取毫秒级数目
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
	cout << "输入拍号"<<endl;
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
	cout << "按下key" << endl;
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