#ifndef ID_GEN_HPP
#define ID_GEN_HPP

 class CIDGen
 {
    public:
     static int GetID();

    private:
        CIDGen(const CIDGen&r)
        {
        }

        static int vID;
 };

#endif