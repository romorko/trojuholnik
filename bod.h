#ifndef BOD_V1_BOD_H
#define BOD_V1_BOD_H
#include <iosfwd>
#include <exception>

class Bod2D{
private:
    float x;
    float y;
public:
    Bod2D():x(0),y(0){};
    explicit Bod2D(float val):x(val),y(val){};
    Bod2D(float valX, float valY):x(valX),y(valY){};
    float getX()const{return x;} ;
    float getY()const{return y;} ;
    float getDistance(const Bod2D & other = Bod2D(0,0)) const;
    Bod2D getCenterPoint(const Bod2D & other) const;
    void setX(float valX){x=valX;};
    void setY(float valY){y=valY;};
    Bod2D operator+(const Bod2D & other) const;
    Bod2D operator-(const Bod2D & other) const;
    Bod2D operator*(float n) const;
    Bod2D operator/(float n) const;

    friend Bod2D operator*(float n, const Bod2D & other);
    friend std::ostream & operator<<(std::ostream & os, const Bod2D & bod);
    friend std::istream & operator>>(std::istream & is, Bod2D & bod);
    static void getMinDistance(std::istream & is);
    static void getSortedDistance(std::istream & is, std::ofstream &os);
    Bod2D getJednotkovy() const;
    int generuj(int min,int max) const;
};

using Vektor = Bod2D;
class Usecka
{
private:
    Bod2D X{0, 0};
    Bod2D Y{0, 0};
public:
    class VR
    {
    private:
        float koeficienty[3];//koeficienty a,b,c rovnice ax+by+c=0
    public:
        VR(float ka, float kb, float kc):koeficienty{ka,kb,kc}{};
        friend std::ostream &operator<<(std::ostream & os,const VR & other);
        float & operator[](int index){return koeficienty[index];};  //pretazeny operator [] pre nekonstantny aj konstantny pristup
        const float & operator[](int index) const{return koeficienty[index];}
    };
    class PR
    {
    private:
        float koeficienty[4];//  koeficienty a1,s1,a2,s2 v rovniciach:  x = a1 + s1*t   y= a2 + s2*t
    public:
        PR(float ka1,float kv1,float ka2,float kv2):koeficienty{ka1,kv1,ka2,kv2}{};
        friend std::ostream &operator<<(std::ostream & os,const PR & other);
        float & operator[](int index){return koeficienty[index];}; //pretazeny operator [] pre nekonstantny aj konstantny pristup
        const float & operator[](int index) const{return koeficienty[index];}

    };
    class Poloha
    {
    private:
        char popis[11]{};
        Bod2D priesecnik;
    public:
        Poloha(char const *text, const Bod2D& prienik);
        Bod2D getpriesecnik()const{return priesecnik;};
        friend std::ostream &operator<<(std::ostream &os, const Poloha &poloha);
    };
    Usecka():X({0,0}),Y({0,0}){};
    Usecka(Bod2D A, Bod2D B) : X(A), Y(B){};
    explicit Usecka(Bod2D A) : X(A), Y(A){};
    friend std::ostream &operator<<(std::ostream &os, const Usecka &usecka);
    friend std::istream &operator>>(std::istream &is, Usecka &usecka);
    bool operator<(const Usecka &other) const;
    bool operator>(const Usecka &other) const;
    explicit operator VR() const;  //konverzia na vseobecnu rovnicu
    explicit operator PR() const;  //konverzia na parametricku rovnicu
    explicit operator float() const; //konverzia na float t.j. na velkost usecky
    float getDlzka() const;
    Vektor getNormalovy() const;
    Vektor getSmerovy() const;
    Bod2D getCenter() const;
    VR getVseobecna() const;
    PR getParametricka() const;
    Usecka getOsStrany() const;
    Usecka getOsUhla(const Usecka & other) const;
    bool jeRovnobezna(const Usecka & other) const;
    bool jeTotozna(const Usecka & other) const;
    float getUhol(const Usecka & other, char typ='s') const;
    Poloha getPoloha(const Usecka & other) const;


};

class Trojuholnik
{
    class MsgErr : public std::exception
    {
    private:
        const char * msg;
    public:
        explicit MsgErr( const char * sprava):msg(sprava){};
        void getMsg() const;
    };
private:
    Bod2D A;
    Bod2D B;
    Bod2D C;
    static bool existuje(Bod2D x, Bod2D y, Bod2D z);
    static int generuj(int min, int max);
public:
    Trojuholnik(Bod2D x, Bod2D y, Bod2D z);
    Trojuholnik();
    float getVelkostStrany(char strana='a') const;
    float getVelkostUhla(char * uhol) const;
    void ukazStrany() const;
    void ukazUhly() const;
    float getObvod() const;
    float getObsah() const;
    Bod2D getTazisko() const;
    Bod2D getOrtocentrum() const;
    void getOpisanaKruznica() const;
    void getVpisanakruznica() const;
    void getKruznicaDeviatichBodov() const;
    Usecka getEulerovaPriamka() const;
    Usecka getVyska(char naStranu) const;
    Usecka getTaznica(char naStranu) const;
};


#endif //BOD_V1_BOD_H
