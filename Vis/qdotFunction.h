#include "qdotFile.h"

extern void redisplay_all();

//extern void fdatapop(int id);
extern void fdataok(int id);
extern void fdatacancel(int id);
extern void fdataprocessing();
//extern bool fdatacompare(int id);

extern void farrowprocessing();

extern void fcontoursync(int id);
extern void fcontourpaser();
extern void fcontourprocessing();
//extern void fcontourpop();
extern void fcontouredit(int id);
//extern void fcontourok();
//extern void fcontourcancel();

/*
extern void fsymmetrymove();
extern void fsymmetryparser();
extern void fsymmetrypop();
extern void fsymmetryedit(int id);
extern void fsymmetryok();
extern void fsymmetrycancel();
*/

extern void fclustersync(int id);
extern void fclustertype(int id);
extern void fcuttingedit(int id);
extern void fcuttingpaser(int id);
extern void fcuttingeditpaser(int id);
extern void fcuttingmove(int id);
extern void fcuttingprocessing();
extern void fclusterpaser(int id);
//extern void fclusterpop();
extern void fclusteredit(int data_type, int id);
extern void fclusterlayer(int id);
extern void fclusterweight(int data_type, int id);
extern void fclusterprocessing();
//extern void fclusterok();
//extern void fclustercancel();

extern void fdistancesync(int id);
extern void fdistancesmaple();
extern void fdistancepaser();
extern void fdistanceprocessing();
extern void fdistancez(int id);
//extern void fdistancepop();
extern void fdistanceedit(int id);
//extern void fdistanceok();
//extern void fdistancecancel();

//extern void froimove(int id);

//extern void fcuttingmove();
//extern void fcuttingedit(int id);

extern void fsetcolor();
extern void fsetdisc();

extern void fbackground();
extern void fopacity();
extern void farrow();
extern void flength();
extern void fblind();
extern void ffrequency();

extern void fencode(int id);
//extern void fprojection(int id);

extern void farrowbase();
extern void fclusterdisc();
extern void fsetcolor();
extern void fscalar();
extern void fresetarrow();
extern void fsetdisplaylist_arrow();
extern void fsetdisplaylist_mesh();
extern void toggleencode();

extern void flayerclusteredit();
extern void flayerclusterupdate(int id);

//==temporal====================
extern void newcluster();
extern void increasescaling();
extern void increasealpha();
extern void increasearrowbase();
extern void decreasescaling();
extern void decreasealpha();
extern void decreasearrowbase();
//=================================

extern void fupdate();


extern void init();
extern void control(int id);
