#include <iostream>
#include <cmath>
using namespace std;

int UNOpuntoX;
int UNOpuntoY;
int DUEpuntoX;
int DUEpuntoY;
char caratterePiano = '*';
char caratterePunto = '*';

int ordinate = 10;
int ascisse = 10;

void Inserimento()
{
	cout<<"inserisci il primo punto\n";
	cout<<"X: ";
	cin>>UNOpuntoX;
	cout<<"Y: ";
	cin>>UNOpuntoY;
	cout<<"inserisci il secondo punto\n";
	cout<<"X: ";
	cin>>DUEpuntoX;
	cout<<"Y: ";
	cin>>DUEpuntoY;
	
	cout<<endl<<endl<<endl;
}

void PianoCartesiano()
{
	for(int i = ascisse; i > 0; i --)
	{
	  cout<<caratterePiano;
	  for(int j = 1; j < ordinate-1; j++){
			if(UNOpuntoX == j && UNOpuntoY == i){
				cout<<caratterePunto;
			}else if(DUEpuntoX == j && DUEpuntoY == i){
				cout<<caratterePunto;
			}
			if(j > 5){
				cout<<" ";
			}else{
				cout<<"   ";
			}
	  }
	  cout<<endl;
	}
	for(int i = 0; i< ordinate; i++){
		cout<<caratterePiano<<" ";
	}
}

void CalcoloDistanza()
{
	int distanza;
	float radiceQuadrata;
	
	if(UNOpuntoX == DUEpuntoX){
		distanza = UNOpuntoY - DUEpuntoY;
		cout<<"La distanza e': "<<distanza;
	}else if(UNOpuntoY == DUEpuntoY){
		distanza = UNOpuntoX - DUEpuntoX;
		cout<<"La distanza e': "<<distanza;
	}else{
		distanza = ((UNOpuntoX - DUEpuntoX)*(UNOpuntoX - DUEpuntoX))+((UNOpuntoY-DUEpuntoY)*(UNOpuntoY-DUEpuntoY));
		radiceQuadrata = sqrt(distanza);
		cout<<"La distanza e': "<<radiceQuadrata;
	}
}

int main()
{
	while(true)
	{
		Inserimento();
		PianoCartesiano();
		cout<<endl<<endl;
		CalcoloDistanza();
		cout<<endl;
		system("pause");
		system("CLS");
	}
	return 0;
}
