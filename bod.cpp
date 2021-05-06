#include "bod.h"
#include "inout.h"
#include <cmath>
#include <cstring>
#include <random>

using namespace inout;

int comp(const void *prva,const void *druha );

float Bod2D::getDistance(const Bod2D &other) const
{
    return (float)sqrt(pow(other.x-x, 2)+pow(other.y-y, 2));
}

Bod2D Bod2D::getCenterPoint(const Bod2D &other) const {
    return {(x + other.x) / 2,(y + other.y) / 2};
}

Bod2D Bod2D::operator+(const Bod2D &other) const {
    return {x+other.x, y+other.y};
}

Bod2D Bod2D::operator-(const Bod2D &other) const {
    return {x-other.x, y-other.y};
}

Bod2D Bod2D::operator*(float n) const {
    return {x*n, y*n};
}

Bod2D Bod2D::operator/(float n) const {
    return {x/n, y/n};
}

std::ostream &operator<<(std::ostream &os, const Bod2D &bod) {
    os << "[" << bod.x << "," << bod.y << "]";
    return os;
}

Bod2D operator*(float n, const Bod2D &other) {
    return {other.x*n, other.y*n};
}

void Bod2D::getMinDistance(std::istream &is)
{
    Bod2D A;
    Bod2D B;
    is>>A>>B;
    Bod2D Temp1;
    Bod2D Temp2;
    float min = A.getDistance(B);
    float tmp;
    for (int i = 0; i < 99; ++i)
    {
        if(is>>A>>B,(tmp=A.getDistance(B)) < min)
        {
            Temp1=A;
            Temp2=B;
            min=tmp;
        }
    }
    std::cout<<"Najmensia vzdialenost je "<<min<<" bodov "<<Temp1<<" "<<Temp2;
}

void Bod2D::getSortedDistance(std::istream &is, std::ofstream &os)
{
    Usecka Usecky[100];
    for(auto & i : Usecky)
    {
        is>>i;
    }
    qsort(Usecky,100,sizeof(Usecka),comp);
    std::cout<<"Utriedene podla velkosti"<<std::endl;
    for(auto i : Usecky)
    {
        std::cout<<(float )i<<" "<<i;
        os<<(float)i<<" "<<i;
    }
}

std::istream &operator>>(std::istream &is, Bod2D &bod) {
    is >> bod.x>>bod.y;
    return is;
}

Bod2D Bod2D::getJednotkovy() const
{
    return Bod2D {this->x/this->getDistance(),this->y/this->getDistance()};
}

int Bod2D::generuj(int min, int max) const
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<int> distr(min, max); // define the range
    return distr(eng); //generate number
}

std::ostream &operator<<(std::ostream &os, const Usecka &usecka)
{
    os<<"Usecka je dana bodmi:"<<usecka.X<<usecka.Y<<std::endl;
    return os;
}

bool Usecka::operator<(const Usecka &other) const
{
    return  (float)(*this)<(float)other;
}

float Usecka::getDlzka() const
{
    return X.getDistance(Y);
}

std::istream &operator>>(std::istream &is, Usecka &usecka)
{
    is>>usecka.X>>usecka.Y;
    return is;
}

bool Usecka::operator>(const Usecka &other) const
{
    //return (float)(*this)>(float)other;
    return this->getDlzka()>other.getDlzka();
}

Vektor Usecka::getNormalovy() const
{
    Vektor tmp =Y-X;
    return {-tmp.getY(),tmp.getX()};
}

Vektor Usecka::getSmerovy() const
{
    return Y-X;
}

Bod2D Usecka::getCenter() const
{
    return X.getCenterPoint(Y);
}

int comp(const void *prva,const void *druha )
{
    auto * A = (Usecka *)prva;
    auto * B = (Usecka *)druha;
    return ((*A)<(*B))?1:((*A)>(*B))?-1:0;
}

std::ostream &operator<<(std::ostream &os, const Usecka::VR &other)
{
    os<<"Vseobecna rovnica:"<<setw(4)<<setprecision(2)<<other[0]<<"x"<<std::showpos<<setw(4)<<other[1]<<"y"<<std::showpos<<setw(4)<<other[2]<<" = 0"<<endl;
    return os;
}

Usecka::VR Usecka::getVseobecna() const
{
    float a = this->getNormalovy().getX();
    float b = this->getNormalovy().getY();
    float c= -a*X.getX()-b*X.getY();
    //cout<<Usecka::VR(a, b, c);
    return {a,b,c};
}

Usecka::operator VR() const
{
    return getVseobecna();
}

Usecka::operator float() const
{
    return X.getDistance(Y);
}

std::ostream &operator<<(std::ostream &os, const Usecka::PR &other)
{
    os<<"Parametricka rovnica:"<<std::endl<<"x = "<<setw(4)<<std::noshowpos<<other[0]<<std::showpos<<setw(4)<<other[1]<<"*t"<<std::endl<<"y = "<<setw(4)<<std::noshowpos<<other[2]<<std::showpos<<setw(4)<<other[3]<<"*t"<<"   t je z R"<<std::endl;
    return os;
}

Usecka::PR Usecka::getParametricka() const
{
    float s1=this->getSmerovy().getX();//x = a1 + s1*t   y= a2 + s2*t
    float s2 =this->getSmerovy().getY();
    float a1=X.getX();
    float a2=X.getY();
    std::cout<<Usecka::PR(a1, s1, a2, s2);
    return {a1, s1, a2, s2};
}

Usecka::operator PR() const
{
    return getParametricka();
}

Usecka Usecka::getOsStrany() const
{
    Bod2D stred=getCenter();
    return {stred,stred+getNormalovy()} ;//smerovy vektor usecky je normalovy vo vseobecnej rovnici
}

float Usecka::getUhol(const Usecka &other, char typ) const
{
    Vektor vektor1 = this->getSmerovy();
    Vektor vektor2 = other.getSmerovy();
    auto cislo = (vektor1.getX()*vektor2.getX()+vektor1.getY()*vektor2.getY())/((vektor1.getDistance()) *vektor2.getDistance());
    //(typ=='o')?cout<<"Uhol je "<<noshowpos<<acos(cislo)<<" rad alebo ":cout<<acos(cislo)*(180/3.14)<<" st";
    return (typ=='o')?static_cast<float>(std::acos(cislo)):static_cast<float>(std::acos(cislo)*(180/3.14));
}

bool Usecka::jeRovnobezna(const Usecka &other) const
{
    Vektor s1= this->getSmerovy();
    Vektor s2= other.getSmerovy();
    return (s1.getX() / s2.getX() == s1.getY() / s2.getY());
}

bool Usecka::jeTotozna(const Usecka &other) const
{
    VR prva = this->getVseobecna();
    VR druha = other.getVseobecna();
    return (prva[0]/druha[0])==(prva[1]/druha[1])==(prva[2]/druha[2]);
}

Usecka::Poloha Usecka::getPoloha(const Usecka &other) const
{
    if(this->jeTotozna(other))
    {
        return Usecka::Poloha {"totozne", Bod2D{0,0}};
    }
    if(this->jeRovnobezna(other))
    {
        return Usecka::Poloha {"rovnobezne", Bod2D{0,0}};
    }
    auto prva = this->getVseobecna();
    auto druha = other.getVseobecna();
    float D = prva[0] * druha[1] - prva[1] * druha[0];
    float D1 = -prva[2] * druha[1] - prva[1] * druha[2] * (-1);
    float D2 = -prva[0] * druha[2] - druha[0] * prva[2] * -1;
    return Usecka::Poloha {"roznobezne", {D1/D,D2/D}};
}

Usecka::Poloha::Poloha(char const *text, const Bod2D &prienik) : priesecnik(prienik)
{
    std::strncpy(popis,text,10);
    popis[10]='\0';
}

std::ostream &operator<<(std::ostream &os, const Usecka::Poloha &poloha)
{
    os<<"Priamky su "<<noshowpos<<poloha.popis<<". ";
    if(strcmp(poloha.popis,"roznobezne")==0)
    {
        os<<"Priesecnik "<<poloha.priesecnik<<endl;
    }
    return os;
}

Usecka Usecka::getOsUhla(const Usecka &other) const
{
    Bod2D prvyBod = this->getPoloha(other).getpriesecnik();
    Vektor vektor1 = this->getSmerovy().getJednotkovy();
    Vektor vektor2 = other.getSmerovy().getJednotkovy();
    Bod2D druhyBod= vektor1+vektor2+prvyBod;
    return {prvyBod,druhyBod};
}

int Trojuholnik::generuj(int min, int max)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    //std::uniform_real_distribution<float> distr(min,max);
    std::uniform_int_distribution<int> distr(min,max);
    return distr(eng);
}

bool Trojuholnik::existuje(Bod2D x, Bod2D y, Bod2D z)
{
    float a=x.getDistance(y);
    float b=x.getDistance(z);
    float c=y.getDistance(z);
    try
    {
        cout<<"Tri body:"<<x<<y<<z<<" ";
        if(!((a+b>c) * (a+c>b) * (b+c)>a))
        {
            throw Trojuholnik::MsgErr("Trojuholnik s vrcholmi v tychto bodoch neexistuje!");
        }
        cout<<"Trojuholnik OK."<<endl;
    }
    catch(const Trojuholnik::MsgErr &e)
    {
        e.getMsg();
        return false;
    }
    return true;
}

Trojuholnik::Trojuholnik()
{
    Bod2D X {static_cast<float>(generuj(1,10)),static_cast<float>(generuj(1,10))};
    Bod2D Y {static_cast<float>(generuj(1,10)),static_cast<float>(generuj(1,10))};
    Bod2D Z {static_cast<float>(generuj(1,10)),static_cast<float>(generuj(1,10))};
    if(!existuje(X,Y,Z))
    {
        exit(EXIT_FAILURE);
    }
    A=X;
    B=Y;
    C=Z;
}

Trojuholnik::Trojuholnik(Bod2D X, Bod2D Y, Bod2D Z)
{
    if(!existuje(X,Y,Z))
    {
        exit(EXIT_FAILURE);
    }
    A=X;
    B=Y;
    C=Z;
}

float Trojuholnik::getVelkostStrany(char strana) const
{
    float vysledok;
    while(true)
    {
        try
        {
            switch(strana)
            {
                case 'a':
                    vysledok=B.getDistance(C);
                    break;
                case 'b':
                    vysledok=A.getDistance(C);
                    break;
                case 'c':
                    vysledok=A.getDistance(B);
                    break;
                default:
                    throw Trojuholnik::MsgErr("Taka strana nie je!");
            }
            return vysledok;
        }
        catch (const Trojuholnik::MsgErr &e)
        {
            e.getMsg();
            cout<<" Zadaj stranu:";
            cin>>strana;
            continue;
        }
    }
}

float Trojuholnik::getVelkostUhla(char * uhol) const
{
    float vysledok;
    while(true)
    {
        try
        {
            if (strcmp(uhol,"alfa")==0)
            {
                vysledok=Usecka(A,B).getUhol(Usecka(A,C));
            }
            else if(strcmp(uhol,"beta")==0)
            {
                vysledok=Usecka(B,A).getUhol(Usecka(B,C));
            }
            else if(strcmp(uhol,"gama")==0)
            {
                vysledok=Usecka(C,A).getUhol(Usecka(C,B));
            }
            else
            {
                throw Trojuholnik::MsgErr("Taky uhol v trojuholniku ABC nie je!");
            }
            return vysledok ;
        }
        catch (const Trojuholnik::MsgErr & e)
        {
            e.getMsg();
            cout<<" Zadaj spravny uhol (alfa,beta,gama): ";
            cin>>uhol;
            continue;
        }
    }

}

void Trojuholnik::ukazStrany() const
{
    cout<<"Strany trojuholnika: ";
    cout<<setw(5)<<setprecision(3)<<"a= "<<getVelkostStrany('a')<<setw(5)<<"b="<<getVelkostStrany('b')<<setw(5)<<"c="<<getVelkostStrany('c')<<endl;
}

void Trojuholnik::ukazUhly() const
{
    cout<<"Uhly trojuholnika:(stupne) ";
    cout<<setw(5)<<setprecision(3)<<"alfa = "<<getVelkostUhla((char *)"alfa")<<setw(5)<<" beta = "<<getVelkostUhla((char *)"beta")<<setw(5)<<" gama = "<<getVelkostUhla((char *)"gama")<<endl;
    //cout<<"Kontrolny sucet:"<<getVelkostUhla("alfa")+getVelkostUhla("beta")+getVelkostUhla("gama");
}

Bod2D Trojuholnik::getTazisko() const
{
    Bod2D stred1=A.getCenterPoint(B);
    return stred1+(C-stred1)*1/3;
}

Bod2D Trojuholnik::getOrtocentrum() const
{
    return Usecka(getVyska('a')).getPoloha(getVyska('b')).getpriesecnik();
}

float Trojuholnik::getObvod() const
{
    return getVelkostStrany('a')+getVelkostStrany('b')+getVelkostStrany('c');
}

float Trojuholnik::getObsah() const
{
    float s = getObvod()/2;
    return (float)std::sqrt(s*(s-getVelkostStrany('a'))*(s-getVelkostStrany('b'))*(s-getVelkostStrany('c')));
}

void Trojuholnik::getOpisanaKruznica() const
{
    Usecka osAB = Usecka(A, B).getOsStrany();
    Usecka osBC = Usecka(B, C).getOsStrany();
    Bod2D stredKruznice = osAB.getPoloha(osBC).getpriesecnik();
    float polomerKruznice= stredKruznice.getDistance(A);
    cout<<showpos<<"Opisana kruznica: (x"<<setprecision(2)<<stredKruznice.getX()<<")^2 + (y"<<setprecision(2)<<stredKruznice.getY()<<")^2 = "<<setprecision(2)<<noshowpos<<polomerKruznice*polomerKruznice<<endl;
}

void Trojuholnik::getVpisanakruznica() const
{
    Usecka osAlfa = Usecka(A,B).getOsUhla(Usecka(A,C));
    Usecka osBeta = Usecka(B,A).getOsUhla(Usecka(B,C));
    Bod2D stredKruznice = osAlfa.getPoloha(osBeta).getpriesecnik();
    Usecka vyskaC = getVyska('c');
    Bod2D pataVyskyNaA = vyskaC.getPoloha(Usecka(A,B)).getpriesecnik();
    float polomerKruznice = stredKruznice.getDistance(pataVyskyNaA);
    cout<<showpos<<"Vpisana kruznica: (x"<<setprecision(2)<<stredKruznice.getX()<<")^2 + (y"<<setprecision(2)<<stredKruznice.getY()<<")^2 = "<<setprecision(2)<<noshowpos<<polomerKruznice*polomerKruznice<<endl;
}

Usecka Trojuholnik::getTaznica(char naStranu) const
{
    switch (naStranu)
    {
        case 'a':
            return { {B.getCenterPoint(C)},A};
        case 'b':
            return { {A.getCenterPoint(C)},B};
        case 'c':
            return { {A.getCenterPoint(B)},C};
        default:
            return { {B.getCenterPoint(C)},A};
    }

}

Usecka Trojuholnik::getVyska(char naStranu) const
{
    Vektor smerovy;
    Bod2D bodNaVyske;
    switch (naStranu)
    {
        case 'a':
            smerovy = Usecka(C,B).getNormalovy();
            bodNaVyske= {smerovy+A};
            return {bodNaVyske,A};
        case 'b':
            smerovy = Usecka(A,C).getNormalovy();
            bodNaVyske = {smerovy+B};
            return {bodNaVyske,B};
        case 'c':
            smerovy = Usecka(A,B).getNormalovy();
            bodNaVyske = {smerovy+C};
            return {bodNaVyske,C};
        default:
            smerovy = Usecka(C,B).getNormalovy();
            bodNaVyske= {smerovy+A};
            return {bodNaVyske,A};
    }
}

void Trojuholnik::getKruznicaDeviatichBodov() const
{
    Bod2D stredAB = A.getCenterPoint(B);
    Bod2D stredAC = A.getCenterPoint(C);
    Bod2D stredBC = B.getCenterPoint(C);
    Usecka tetiva1 = Usecka(stredAB,stredAC).getOsStrany();
    Usecka tetiva2 = Usecka(stredAC,stredBC).getOsStrany();
    Bod2D stredKruznice = tetiva1.getPoloha(tetiva2).getpriesecnik();
    float polomerKruznice = stredKruznice.getDistance(stredAB);
    cout<<showpos<<"Kruznica 9 bodov: (x"<<setprecision(2)<<stredKruznice.getX()<<")^2 + (y"<<setprecision(2)<<stredKruznice.getY()<<")^2 = "<<setprecision(2)<<noshowpos<<polomerKruznice*polomerKruznice<<endl;
}

Usecka Trojuholnik::getEulerovaPriamka() const
{
    return {getOrtocentrum(),getTazisko()};
}

void Trojuholnik::MsgErr::getMsg() const
{
    cout<<msg;
}

