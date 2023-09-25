#include <iostream>

using namespace std;


/*****************************************************************************************************************/
//STRUKTURA W TABLICY HASZOWANEJ - DO ZLICAZNIA SYMBOLI

struct hasz_symbol{
  char * wzor;
  int lengt;
  hasz_symbol * next=nullptr;
  int licznik;
};


//FUNKCJA HASZUJACA

int hf(char *symbol, unsigned int dlug)
{
  unsigned int h,i;

  h = 0;
  for(i = 0; i < dlug; i++)
    h = 2 * h + 1 - (symbol[i] & 1);
  return h % 50;
}

//FUNKCJA DODAJACA ODNALEZIONE SYMBOLE DO TABLICY HASZOWANEJ

void dodaj_do_haszowanej (char *symbol, hasz_symbol **tablica_haszowana,int dlug, int hf)
{
 // 1 - GDY NIE MA KOLIZJI

    if (tablica_haszowana[hf] == nullptr){
        tablica_haszowana[hf]= new hasz_symbol;
        tablica_haszowana[hf]->lengt=dlug;
        tablica_haszowana[hf]->licznik=1;
        tablica_haszowana[hf]->next=nullptr;
        tablica_haszowana[hf]->wzor = new char[dlug];
        for (int j=0; j<dlug; j++)
            tablica_haszowana[hf]->wzor[j] = symbol[j];
     return;
    }


 // 2 -GDY SA KOLIZJE

    hasz_symbol *temp = tablica_haszowana[hf];
    bool identyczne;

    do
        {
            //NAJPIERW SPRAWDZ CZY SYMBOL SIE POWTARZA
            identyczne = true;
            for (int j=0; j < temp->lengt-1; j++)
                 if (temp->wzor[j] != symbol[j]){
                    identyczne = false;
                    break;
                 }
            //GDY SYMBOL SIE POWTARZA TO ZWIEJSZ LICZNIK
            if (identyczne == true)
            {
             temp->licznik++;
             return;
            }
            //GDY SIE NIE POWTARZA Z OBECNYM TO:
              //SPRAWDZ CZY MA NASTEPNIKA A JAK NIE TO GO STWORZ - I DODAJ SYMBOL
            if (temp->next == nullptr)
            {
                    temp->next = new hasz_symbol;
                    temp->next->lengt = dlug;
                    temp->next->licznik = 1;
                    temp->next->next = nullptr;
                    temp->next->wzor = new char[dlug];
                    for (int j=0; j<dlug-1; j++)
                        temp->next->wzor[j] = symbol[j];
                    return;
            }
            //JESLI MA NASTEPNIKA TO IDZ DO NIEGO I KONTYNUUJ PETLE SPRAWDZAJACA
            temp = temp->next;
            continue;
        }
        while (temp != nullptr);
}


/*************************************************************************************************************************/

//MOJE PIERWSZE PODEJSCIE DO OBIEKTOW - KLASA ZBIOR (DESTRUKTOR DOMYSLNY)


class Zbior{
    public:
        int i;
        int j;
        int start_z=0;

        short dlug=0;
        bool **macierz;
        char *symbol=nullptr;

        /******************************************************/
        //TWORZE MACIERZ WEJSCIOWA
        Zbior(int w, int h){
             j=w;
             i=h;
             macierz = new bool *[h];
             for (int z=0; z<h; z++)
                macierz[z]= new bool[w];
        }
        /*******************************************************/
        void dodaj_pkty(){
        //DODAJE DO MACIERZY PUNKTY BOOL

             for (int z=0; z< i; z++){
             char linijka[1005]={};
                  cin>>linijka;
                  for (int k=0; k<j; k++)
                  macierz[z][k]=linijka[k]-'0';
             }
        }
        /***********************************************************/
        //NAJWAZNIEJSZA FUNKCJA _ KORZYSTA Z NASTPNEJ - ROWNIE WAZNEJ
        void * wyluskaj_kolejny_symmbol(){

           if (symbol != nullptr ) delete symbol;
           dlug=0;
           symbol = new char[10000];
           symbol[0]='s';

            for (int z=start_z; z< i; z++){
                for (int k=0; k<j; k++){
                   dlug=0;
                   dfs(z,k, 0);
                   if (dlug!=0) {
                       return 0;}
                }
                start_z++;
                if (dlug!=0)
                    return 0;
            }
            return 0;
        };
/************************************************************************/
        //PRZESZUKIANIE SYMBOLI W GLAB - POKI CO TYLKO DO 2000 ZNAKOW BO PRZEKRACZA LIMIT STOSU (KOLEJKE TRZEBA
        //DODAC DO TEGO  CHYBA)
        void dfs( short i, short j,short kierunek){
            if ((i<0) || (i>=this->i)) return;
            if ((j<0)||(j>=this->j)) return;
            if (macierz[i][j]==0)
                return;
            if (dlug==2000) return;
            symbol[dlug]=kierunek + '0';
            dlug++;

            macierz[i][j]=0;

            dfs(i-1,j,8);
            dfs(i+1,j,2);
            dfs(i,j-1,4);
            dfs(i,j+1,6);

            symbol[dlug]='0';
            dlug++;

        return;
        }


};

//KONIEC KLASY KONIEC KLASY

/***************************************************************************************************************************/
//LISTA JEDNOKIERUNKOWA DO STOSU ALE TEZ DO ..... (CHYBA JA WRZUCE DO SRODKA KLASY)
struct Slist{
    char litera='k';
    Slist *next;

};
/**********************************************************************/
//MOJA DRUGA KLASA  - STOS (KONSTRUKTOR I DESTRUKTOR DOMYSLNY
class Tstos
{
    private:
        Slist *stosik;
    public:
        Slist *top(void);
        void push (char o);
        void pop(void);

};

/************************************************************************/
Slist * Tstos::top(void){
    return stosik;
}
void Tstos::push(char o){
        Slist * temp = new Slist;
        temp->litera = o;
        temp->next= this->stosik;
        this->stosik = temp;
}
void Tstos::pop(void){
        if (this->stosik != nullptr){
        Slist *temp2 = this->stosik;
        this->stosik = this->stosik->next;
        delete temp2;
        temp2=nullptr;
        return;
    }
return;
}

/**************************************************************************************************************************/
/****************************************************************************************************************************/
/*******************************************************************************************************************************/

int main()
{
    //TABL HASZOWANA DO ZLICZENIE ROZNYCH SYMBOLI
    int hasz = 0;
    hasz_symbol *tablica_haszowana[50];
    for (int i=0; i<50; i++)
       tablica_haszowana[i] = nullptr;

    //STOS DO PRZECHOWYWANIA KIERUNKOW ZGODNIE Z KTORYMI BEDZIEMY WYPISYWAC ODPOWIEDZ KONCOWA
    Tstos Stack;
    int w,h;

    //WEJSCIE
    cin>>w;
    cin>>h;
    Zbior Wejscie(w,h);
    Wejscie.dodaj_pkty();

    // ZNAJDZ SYMBOL I GO WYPISZ tzn - ZIDENTYFIKUJ WSZYSTKIE SYMBOLE NA WEJSCIU, WPISZ DO TABLICY HASZOWANEJ ICH KSZTALTY
    //I JE ZLICZ, PO CZYM WYPISZ NAJLICZNIEJSZY SYMBOL)

    Wejscie.wyluskaj_kolejny_symmbol();        // IDENTYYFIKACJA SYMBOLI - NAJTRUDNIEJSZA CZESC PROGRAMU

    while (Wejscie.symbol[0] != 's'){
        hasz =  hf( Wejscie.symbol, Wejscie.dlug);
        dodaj_do_haszowanej(Wejscie.symbol,tablica_haszowana,Wejscie.dlug,hasz);   //DODAWANIE DO HASZOWANEJ TABLICY
        if (Wejscie.symbol== nullptr)
            delete Wejscie.symbol;
         Wejscie.wyluskaj_kolejny_symmbol();                                        //TRZEBA ZIDENTYFIKOWAC WSZYSTKIE SYMBOLE
    }

    //ZNAJDZ NAJLICZNIEJSZY SYMBOL
    int liczn_max = 1;
    hasz_symbol *wsk_na_maxa;
    hasz_symbol * temp_wsk;
    for (int i=0; i<50; i++){
        temp_wsk = tablica_haszowana[i];
        while (temp_wsk != nullptr){
            if (temp_wsk->licznik > liczn_max) {
               liczn_max = temp_wsk->licznik;
               wsk_na_maxa=temp_wsk;
            }
            temp_wsk = temp_wsk->next;
        }
    }

//PIERWSZA ODPOWIEEDZ - PODAJ LICZBE WYSTAPIEN NAJLICZNIEJSZEGO SYMBOLU
cout<<liczn_max<<endl;

//DRUGA ODPWOWIEDZ
//WYMAGA ZAMIANY SYMBOLU KTORY NAJLICZNIEJ WYSTEPUJE - OBECNIE ZAPISANY JEST ON W WSK_NA_MAXA
//(W POLU STRUKTURY - JEJ TABLICY CHAR) NA SYMBOL GRAFICZNY W POSTACI TABLICY BOOL


    //TWORZE TABLICE BOOL 4 RAZY WIEKSZA NIZ MAXYMALNY ROZMIAR SYMBOLU I ZACZNE Z JEJ SRODKA
    bool tab[260][260];
    for (int a=0; a<260; a++)
        for (int b=0; b<260; b++)
            tab[a][b]=0;

    int a=130;
    int b=130;

    tab[a][b]=1;

    for (int x=1; x < wsk_na_maxa->lengt-1; x++){

      //JESLI ZNAK W SYMBOLU WSK_NA_MAXA JEST ROZNY OD 0 TO DODAJ NA STOS
        if (wsk_na_maxa->wzor[x] != (0+'0'))
            Stack.push(wsk_na_maxa->wzor[x]);

      //JESLI 0 TO ZDEJMIJ ZE STOSU LITERE CHAR - JEST TO OSTATNI KIERUNEK  I WROC DO JEGO ODWROTNOSCI....
        if (wsk_na_maxa->wzor[x]==0+'0'){
            char kier = Stack.top()->litera;
            Stack.pop();
           //a dokladniej ustaw aktualne wspolrzedne a i b poprawiajac  o kierunek zdjety ze stosu
                if (kier==8+'0')
                    a = a+1;
                if (kier==2+'0')
                    a = a-1;
                if (kier==4+'0')
                    b = b+1;
                if (kier==6+'0')
                    b = b-1;
        }
        //JESLI AKTUALNYM ZNAKIEM W SYMBOLU WSK_NA_MAXA JEST 8 LUB 2....TO PRZESUN SIE W ODPOWIEDNIE MIEJSCE NA KTORE WSKAZUJE ZNAK
        //I PODSTAW 1 (JEDYNKE) W SYMBOLU DO WYPISANIA
        if (wsk_na_maxa->wzor[x] == 8+'0') {
                    a = a-1;
                    tab[a][b]=1;
        }
        if (wsk_na_maxa->wzor[x]==2+'0') {
                a=a+1;
                tab[a][b]=1;
        }
        if (wsk_na_maxa->wzor[x]==4+'0') {
                b=b-1;
                tab[a][b]=1;
        }
        if (wsk_na_maxa->wzor[x]==6+'0') {
                b=b+1;
                tab[a][b]=1;
        }
    }
//PRZYTNIJ RAMKI
        bool puste    =  true;
        int start_a   = -1;
        int koniec_a  = -1;
        int start_b   = -1;
        int koniec_b  = -1;

        for (int a=0; a < 260; a++){
            for (int b=0; b < 260; b++)
                if (tab[a][b] != 0)  puste = false;
            if ((puste == false) && (start_a == -1)) start_a = a;
            if ((puste == true) && (start_a != -1) && (koniec_a == -1)) koniec_a = a;
            puste=true;
            }
        for (int b=0; b < 260; b++){
            for (int a=0; a < 260; a++)
                if (tab[a][b] != 0)  puste = false;

            if ((puste == false) && (start_b == -1)) start_b = b;
            if ((puste == true) && (start_b != -1) && (koniec_b == -1)) koniec_b = b;
            puste=true;
            }
//WYPISZ SYMBOL ZAPISANY W BOOL TABLICY
        for (int a=start_a; a<koniec_a; a++){
            for (int b=start_b; b<koniec_b; b++)
                 cout<<tab[a][b];
            cout<<endl;
            }

return 0;  //koniec maina;
}
