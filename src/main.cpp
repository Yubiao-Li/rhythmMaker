
#include"Maker.h"
#include"AudioClip.h" 

using namespace std;

int main()
{
	//播放音乐
	
	Maker instance;
	string prompt;
	
	prompt = "欢迎使用打谱器，键入1开始打谱，键入2试听谱面,键入3从指定的时间开始试听，键入4修改谱面时间，键入5修改key，键入6删除一个音符";
	cout << prompt << endl;
	int command;
	while (cin >> command)
	{
		cout << endl;
		switch (command)
		{
		case 1:
			instance.begintype();
			break;
		case 2:
			instance.listening_test();
			break;
		case 3:
			cout << "输入从第几拍开始" << endl;
			double star_s;
			cin >> star_s;
			instance.listening_test(star_s);
			break;
		case 4:
			cout << "输入要修改的谱号：";
			int code1;
			while(cin >> code1&&code1==0)
				cout<<"错误的输入值";
			instance.modify_time(code1);
			break;
		case 5:
			cout << "输入要修改的谱号：";
			int code2;
			while (cin >> code2 && code2 == 0)
				cout << "错误的输入值";
			instance.modify_key(code2);
			break;
		case 6:
			cout << "输入要删除的谱号：";
			int num;
			while (cin >> num && num == 0)
				cout << "错误的输入值";
			instance.del(num);
			break;
		default:
			break;
		}
		cout <<prompt<< endl;
	}
	
	
	
	
}