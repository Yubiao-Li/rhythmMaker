
#include"Maker.h"
#include"AudioClip.h" 

using namespace std;

int main()
{
	//��������
	
	Maker instance;
	string prompt;
	
	prompt = "��ӭʹ�ô�����������1��ʼ���ף�����2��������,����3��ָ����ʱ�俪ʼ����������4�޸�����ʱ�䣬����5�޸�key������6ɾ��һ������";
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
			cout << "����ӵڼ��Ŀ�ʼ" << endl;
			double star_s;
			cin >> star_s;
			instance.listening_test(star_s);
			break;
		case 4:
			cout << "����Ҫ�޸ĵ��׺ţ�";
			int code1;
			while(cin >> code1&&code1==0)
				cout<<"���������ֵ";
			instance.modify_time(code1);
			break;
		case 5:
			cout << "����Ҫ�޸ĵ��׺ţ�";
			int code2;
			while (cin >> code2 && code2 == 0)
				cout << "���������ֵ";
			instance.modify_key(code2);
			break;
		case 6:
			cout << "����Ҫɾ�����׺ţ�";
			int num;
			while (cin >> num && num == 0)
				cout << "���������ֵ";
			instance.del(num);
			break;
		default:
			break;
		}
		cout <<prompt<< endl;
	}
	
	
	
	
}