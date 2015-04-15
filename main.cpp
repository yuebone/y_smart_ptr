#include <iostream>
#include "y_smart_ptr.h"
#include <cassert>

using namespace std;


class Base
{
public:
    virtual ~Base(){}

    virtual int foo()const=0;

    virtual int get()const=0;
};

#define PRINT_AC

class A:public Base
{
public:
    int a;
    A(int a_=0):a(a_)
    {
#ifdef PRINT_AC
        std::cout<<"construct A("<<a<<")\n";
#endif
    }

    ~A()
    {
#ifdef PRINT_AC
        std::cout<<"destruct A("<<a<<")\n";
#endif
    }

    virtual int foo()const{ return a*10;}

    virtual int get()const{ return a;}
};

class C:public Base
{
public:
    int c;

    C(int c_=0):c(c_)
    {
#ifdef PRINT_AC
        std::cout<<"construct C("<<c<<")\n";
#endif
    }

    ~C()
    {
#ifdef PRINT_AC
        std::cout<<"destruct C("<<c<<")\n";
#endif
    }

    virtual int foo()const{ return c*100;}

    virtual int get()const{ return c;}
};

class deleteBase
{
public:
    void operator()(Base *b)
    {
        delete b;
    }
};



void delete_A_array(A* ap)
{
    delete [] ap;
}



void test_unique_ptr()
{
    cout << "test unique_ptr" << endl;
    {
        int *ap = new int(10);

        y::unique_ptr<int> uap(ap);

        y::unique_ptr<int> ubp(y::move(uap));

        assert(!uap);
        assert(ubp);
        assert(*ubp == 10);

        //cout<<"% *ubp="<<*ubp<<endl;

        y::unique_ptr<int> ucp = y::move(ubp);
        assert(!ubp);
        assert(ucp);
        assert(*ucp == 10);

        int *bp = new int(20);
        ucp.reset(bp);
        assert(ucp);
        assert(*ucp == 20);

        ucp.reset();
        assert(!ucp);


        y::unique_ptr<int> udp = y::unique_ptr<int>(new int(30));
        assert(*udp == 30);
        y::unique_ptr<int> uep = y::unique_ptr<int>(new int(40));
        assert(*uep == 40);
        uep.swap(udp);
        assert(*udp == 40);
        assert(*uep == 30);
        swap(udp, uep);
        assert(*udp == 30);
        assert(*uep == 40);


        int x = 5;
        C *Cp_ = new C(x);
        A *Ap_ = new A(x);
        y::unique_ptr<Base, deleteBase> uB(Ap_);
        assert(uB->foo() == x * 10);
        uB.reset(Cp_);
        assert(uB->foo() == x * 100);


        int *arr = new int[3];
        arr[0] = 0;
        arr[1] = 1;
        arr[2] = 2;

        A *Aarr = new A[3];
        Aarr[0].a = 10;
        Aarr[1].a = 20;
        Aarr[2].a = 30;

        y::unique_ptr<int[]> uarr(arr);
        y::unique_ptr<A[]> uAarr(Aarr);

        assert(uarr[1] == 1);
        assert(uarr[2] == 2);
        assert(uAarr[0].foo() == (10 * 10));
        assert(uAarr[1].a == 20);

    }
    cout<<"done"<<endl;
}


void test_simple_ptr()
{
    cout<<"test simple_ptr"<<endl;
    {
        y::simple_ptr<int> sap(new int(10));

        assert(sap);
        assert(*sap == 10);

        int *p = sap.release();
        delete p;
        assert(!sap);

        A *Aarr = new A[3];
        Aarr[0].a = 10;
        Aarr[1].a = 20;
        Aarr[2].a = 30;

        y::simple_ptr<A[]> uAarr(Aarr);

        assert(uAarr[0].foo() == (10 * 10));
        assert(uAarr[1].a == 20);
    }
    cout<<"done"<<endl;
}


void test_shared_ptr()
{
    cout<<"test shared_ptr"<<endl;

    int *xp=new int(10);

    y::shared_ptr<int> sp1(xp);
    assert(sp1);
    assert(sp1.unique());
    assert(sp1.use_count()==1);
    assert(sp1.get()==xp);
    assert(*sp1==10);

    y::shared_ptr<int> sp2;
    assert(!sp2);
    assert(!sp2.unique());
    assert(sp2.use_count()==0);
    assert(sp2.get()==0);

    sp2=sp1;
    assert(sp2);
    assert(!sp2.unique());
    assert(sp2.use_count()==2);
    assert(sp1.use_count()==2);
    assert(sp2.get()==xp);
    assert(*sp2==10);

    sp2.reset();
    assert(!sp2);
    assert(sp1);
    assert(sp1.unique());
    assert(sp1.use_count()==1);
    assert(sp1.get()==xp);
    assert(*sp1==10);

    int *zp=new int(20);
    y::shared_ptr<int> sp3(zp);
    sp1.swap(sp3);
    assert(*sp1==20);
    assert(*sp3==10);

    int *yp=new int(30);
    sp1.reset(yp);
    assert(sp1.unique());
    assert(sp1.use_count()==1);
    assert(*sp1==30);



    {
        Base *ap1=new A(11);
        Base *ap2=new A(12);
        y::shared_ptr<Base> bsp1(ap1);
        bsp1.reset(ap2);

        Base *cp1=new C(101);
        y::shared_ptr<Base> bsp2(cp1);
        y::shared_ptr<Base> bsp3(bsp2);

        assert(bsp2.use_count()==2);
        assert(bsp3.use_count()==2);

        bsp2.swap(bsp1);
        assert(bsp1->get()==101);
        assert(bsp2->get()==12);
        assert(bsp3->get()==101);
        assert(bsp1.use_count()==2);
        assert(bsp2.use_count()==1);
        assert(bsp3.use_count()==2);

        bsp1.reset();
        assert(bsp1.use_count()==0);
        assert(bsp2.use_count()==1);
        assert(bsp3.use_count()==1);

        bsp1=bsp2;
        assert(bsp1->get()==12);
        assert(bsp1.use_count()==2);
        assert(bsp2.use_count()==2);
    }
    {
        A* Aarr=new A[3];
        Aarr[0].a=10;
        Aarr[1].a=20;
        Aarr[2].a=30;

        y::shared_ptr<A> a(Aarr,delete_A_array);

        A *ap1=new A(1);
        a.reset(ap1,deleteBase());
    }

    cout<<"done"<<endl;
}



int main()
{
    test_unique_ptr();
    test_simple_ptr();
    test_shared_ptr();

    return 0;
}