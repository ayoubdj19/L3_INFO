#ifndef __PILE__
#define __PILE__

struct noeud {
int donnee;
struct noeud *prochain;
};
typedef struct noeud *pile;

pile pile_vide();
void detruire_pile(pile p);
pile empiler(int donnee, pile);
int sommet(pile p);
pile depiler(pile p);
int est_pile_vide(pile p);

#endif
