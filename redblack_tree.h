//
// Created by Pascal Charpentier on 2025-04-01.
//

#ifndef REDBLACK_TREE_H
#define REDBLACK_TREE_H
#include <cassert>


template<typename K, typename V>
class RedBlackTree {
private:
    enum Color { RED, BLACK };


    struct RedBlackNode {
        Color color;
        RedBlackNode *parent;
        RedBlackNode *gauche;
        RedBlackNode *droite;
        K cle;
        V valeur;

        // Attribut utilisé pour le calcul de l'invariant

        size_t black_height;


        explicit RedBlackNode(Color col) : color(col), parent(nullptr), gauche(nullptr), droite(nullptr),
                                           black_height(0) {
        }

        RedBlackNode(const K &k, const V &v) : color(RED), parent(nullptr), gauche(nullptr), droite(nullptr), cle(k),
                                               valeur(v), black_height(0) {
        }
    };

    using SousArbre = RedBlackNode *;

public:
    RedBlackTree();

    void inserer(const K &cle, const V &valeur);

    void supprimer(const K &cle);

    const V &lire(const K &cle) const;

    std::vector<K> parcourirEnOrdre() const;

private:
    void rotationVersLaGauche(SousArbre root);

    void rotationVersLaDroite(SousArbre root);

    SousArbre transplanter(SousArbre child, SousArbre root) ;

    SousArbre trouverCleDansSousArbre(SousArbre root, const K &cle) const;

    SousArbre maxDansSousArbre(SousArbre root) const;

    SousArbre minDansSousArbre(SousArbre root) const;

    void retablirProprietesApresInsertion(SousArbre &root) ;

    void retablirProprietesApresSuppression(SousArbre root) ;

    void auxParcourirEnOrdre(SousArbre root, std::vector<K> &acc) const;

    bool treeIsOrdered() const;

    void updateBlackHeights(SousArbre root) const;

    size_t nodeBlackHeight(SousArbre root) const;

    bool auxBlackRuleIsEnforced(SousArbre root) const;

    bool blackRuleIsEnforced() const;

    bool redRuleIsEnforced() const;

    bool auxRedRuleIsEnforced(SousArbre root) const;

    bool invariant() const;

private:
    SousArbre nil;
    SousArbre racine;
    size_t cardinal;
};

/***********************************************************************************************************************
 * Méthodes publiques
 **********************************************************************************************************************/

template<typename K, typename V>
RedBlackTree<K, V>::RedBlackTree() : nil(new RedBlackNode(BLACK)), racine(nil), cardinal(0) {
}

template<typename K, typename V>
void RedBlackTree<K, V>::inserer(const K &cle, const V &valeur) {
    auto root_node = racine;
    auto parent_node = nil;

    // Descendre dans l'arbre s'il n'est pas vide

    while (root_node != nil) {
        parent_node = root_node;
        if (cle > root_node->cle) root_node = root_node->droite;
        else if (cle < root_node->cle) root_node = root_node->gauche;
        else throw std::invalid_argument("insertion: clé dupliquée");
    }

    auto *node = new RedBlackNode(cle, valeur);

    // Cas limite : insertion dans un arbre vide...  Par convention : racine sera l'enfant gauche de nil.

    if (parent_node == nil) {
        racine = node;
        nil->gauche = racine ;
        nil->droite = nil ;
    }

    // Cas général : insertion dans un arbre non-vide

    else if (cle > parent_node->cle) parent_node->droite = node;
    else parent_node->gauche = node;

    node->parent = parent_node;
    node->gauche = nil;
    node->droite = nil;

    retablirProprietesApresInsertion(node);
    assert(invariant());
}

template<typename K, typename V>
void RedBlackTree<K, V>::supprimer(const K &cle) {
    auto root_node = racine ;
    auto couleur_supprimee = root_node->color ;

    while (root_node != nil) {

        // Descendre dans l'arbre vers la clé

        if (cle > root_node->cle) root_node = root_node->droite;
        else if (cle < root_node->cle) root_node = root_node->gauche;

        // Clé trouvée !

        else {
            couleur_supprimee = root_node->color ;
            if (root_node->gauche == nil) {

                // Les deux enfants sont nil

                if (root_node->droite == nil) {

                    // Cas général...

                    if (root_node->parent != nil) {
                        if (root_node->parent->gauche == root_node) root_node->parent->gauche = nil ;
                        else root_node->parent->droite = nil ;
                    }

                    // Cas limite : le seul node est la racine
                    else racine = nil ;

                    nil->parent = root_node->parent;
                    delete root_node ;
                    root_node = nil ;
                }

                // Seul l'enfant gauche est nil

                else root_node = transplanter(root_node->droite, root_node) ;
            }

            // Seul l'enfant droite est nil

            else if (root_node->droite == nil) root_node = transplanter(root_node->gauche, root_node) ;

            // Aucun enfant nil, il faut utiliser le successeur

            else {
                auto successeur = minDansSousArbre(root_node->droite) ;
                root_node->cle = successeur->cle ;

                // Le successeur ne peut pas avoir d'enfant à gauche

                couleur_supprimee = successeur->color ;
                root_node = transplanter(successeur->droite, successeur) ;
            }

            // La suppression a eu lieu : rétablir l'invariant, le vérifier et sortir.

            if (couleur_supprimee == BLACK) retablirProprietesApresSuppression(root_node) ;
            assert(invariant()) ;
            return ;
        }
    }

    // Sorti de la boucle while sans avoir trouvé la clé à supprimer

    throw std::invalid_argument("supprimer: clé absente");
}

template<typename K, typename V>
const V &RedBlackTree<K, V>::lire(const K &cle) const {
    auto root_node = racine;

    while (root_node != nil) {
        if (cle > root_node->lireCle()) root_node = root_node->droite;
        else if (cle < root_node->lireCle()) root_node = root_node->gauche;
        else return root_node->lireValeur();
    }
    throw std::invalid_argument("lire: clé non trouvée dans l'arbre");
}

template<typename K, typename V>
std::vector<K> RedBlackTree<K, V>::parcourirEnOrdre() const {
    std::vector<K> acc;
    auxParcourirEnOrdre(racine, acc);
    return acc;
}

/***********************************************************************************************************************
 * Méthodes privées: propriétés redblack
 **********************************************************************************************************************/

template<typename K, typename V>
void RedBlackTree<K, V>::rotationVersLaGauche(SousArbre root) {
    auto newroot = root->droite;
    root->droite = newroot->gauche;
    if (newroot->gauche != nil) newroot->gauche->parent = root;
    newroot->parent = root->parent;

    if (root->parent == nil) racine = newroot;
    else if (root == root->parent->gauche) root->parent->gauche = newroot;
    else root->parent->droite = newroot;

    newroot->gauche = root;
    root->parent = newroot;
}

template<typename K, typename V>
void RedBlackTree<K, V>::rotationVersLaDroite(SousArbre root) {
    auto newroot = root->gauche;
    root->gauche = newroot->droite;
    if (newroot->droite != nil) newroot->droite->parent = root;
    newroot->parent = root->parent;

    if (root->parent == nil) racine = newroot;
    else if (root == root->parent->droite) root->parent->droite = newroot;
    else root->parent->gauche = newroot;

    newroot->droite = root;
    root->parent = newroot;
}

template<typename K, typename V>
typename RedBlackTree<K, V>::SousArbre RedBlackTree<K, V>::transplanter(SousArbre child, SousArbre root) {

    // Cette étape est nécessaire pour que la méthode rétablirProprietesApresSuppression ait accès au bon parent même
    // si le double-black est nil.

    child->parent = root->parent ;

    if (root == root->parent->gauche) root->parent->gauche = child ;
    else root->parent->droite = child ;

    // Libérer le node effacé, si le node remplaçant est en root, il faut mettre à jour l'attribut racine

    delete root ;
    if (child->parent == nil) racine = child ;

    // On va retourner le node remplaçant, cela permettra de mettre l'arbre à jour facilement

    return child ;
}

template<typename K, typename V>
typename RedBlackTree<K, V>::SousArbre RedBlackTree<K, V>::maxDansSousArbre(SousArbre root) const {
    if (root == nil) return nil ;
    while (root->droite != nil) root = root->droite ;
    return root ;
}

template<typename K, typename V>
typename RedBlackTree<K, V>::SousArbre RedBlackTree<K, V>::minDansSousArbre(SousArbre root) const {
    if (root == nil) return nil ;
    while (root->gauche != nil) root = root->gauche ;
    return root ;
}

template<typename K, typename V>
void RedBlackTree<K, V>::retablirProprietesApresInsertion(SousArbre &root) {
    while (root->color == RED && root->parent->color == RED) {
        SousArbre pere = root->parent;
        SousArbre grandpere = pere->parent;

        if (pere == grandpere->gauche) {
            SousArbre oncle = grandpere->droite;
            if (oncle->color == RED) {
                pere->color = BLACK;
                oncle->color = BLACK;
                grandpere->color = RED;
                root = grandpere;
            } else {
                if (root == pere->droite) {
                    rotationVersLaGauche(pere);
                    pere = root;
                }
                rotationVersLaDroite(grandpere);
                pere->color = BLACK;
                grandpere->color = RED;
            }
        } else {
            SousArbre oncle = grandpere->gauche;
            if (oncle->color == RED) {
                pere->color = BLACK;
                oncle->color = BLACK;
                grandpere->color = RED;
                root = grandpere;
            } else {
                if (root == pere->gauche) {
                    rotationVersLaDroite(pere);
                    pere = root;
                }
                rotationVersLaGauche(grandpere);
                pere->color = BLACK;
                grandpere->color = RED;
            }
        }
    }
    racine->color = BLACK;
}

template<typename K, typename V>
void RedBlackTree<K, V>::retablirProprietesApresSuppression(SousArbre root) {
    while (root != racine && root->color == BLACK ) {
        auto parent = root->parent ;

        // Root est enfant gauche
        if (root == parent->gauche) {
            auto frere = parent->droite ;

            // Cas 1 : Frère rouge --> 2, 3, 4
            if (frere->color == RED) {
                rotationVersLaGauche(parent) ;
                parent->color = RED ;
                frere->color = BLACK ;
                frere = parent->droite ;
            }

            // Cas 2 : Frère noir, enfants noirs --> itérations
            if (frere->droite->color == BLACK && frere->gauche->color == BLACK) {
                frere->color = RED ;
                root = parent ;
            }
            else {

                // Cas 3 : Frère noir --> 2 rotations --> Fins
                if (frere->droite->color == BLACK) {
                    rotationVersLaDroite(frere) ;
                    frere->color = RED ;
                    frere->parent->color = BLACK ;
                    frere = frere->parent ;
                }

                // Cas 4 : Frère noir --> 1 rotation --> Fin
                rotationVersLaGauche(parent) ;
                frere->color = parent->color ;
                parent->color = BLACK ;
                frere->droite->color = BLACK ;

                // On veut sortir de la boucle while !
                root = racine ;
            }

        }

        // Root est enfant droite
        else {
            auto frere = parent->gauche ;

            // Cas 1 : Frère rouge --> 2, 3, 4
            if (frere->color == RED) {
                rotationVersLaDroite(parent) ;
                parent->color = RED ;
                frere->color = BLACK ;
                frere = parent->gauche ;
            }

            // Cas 2 : FRère noir, enfants noirs --> itérations
            if (frere->droite->color == BLACK && frere->gauche->color == BLACK) {
                frere->color = RED ;
                root = parent ;
            }
            else {

                // Cas 3 : Frère noir --> 2 rotations --> Fins
                if (frere->gauche->color == BLACK) {
                    rotationVersLaGauche(frere) ;
                    frere->color = RED ;
                    frere->parent->color = BLACK ;
                    frere = frere->parent ;
                }

                // Cas 4 : Frère noir --> 1 rotation --> Fin
                rotationVersLaDroite(parent) ;
                frere->color = parent->color ;
                parent->color = BLACK ;
                frere->gauche->color = BLACK ;

                // Ceci nous force à sortir de la boucle while !
                root = racine ;
            }

        }

    }
    root->color = BLACK ;
}

template<typename K, typename V>
void RedBlackTree<K, V>::auxParcourirEnOrdre(SousArbre root, std::vector<K> &acc) const {
    if (root == nil) return;

    auxParcourirEnOrdre(root->gauche, acc);
    acc.push_back(root->cle);
    auxParcourirEnOrdre(root->droite, acc);
}

/***********************************************************************************************************************
 * Méthodes privées: invariant redblack
 **********************************************************************************************************************/

template<typename K, typename V>
bool RedBlackTree<K, V>::treeIsOrdered() const {
    std::vector<K> result = parcourirEnOrdre();
    if (result.size() < 2) return true;
    for (size_t i = 0 ; i < result.size() - 1 ; ++i)
        if (result.at(i + 1) < result.at(i)) return false;
    return true;
}

template<typename K, typename V>
void RedBlackTree<K, V>::updateBlackHeights(SousArbre root) const {
    if (root == nil) return;
    updateBlackHeights(root->gauche);
    updateBlackHeights(root->droite);
    size_t offset = (root->color == BLACK) ? 1 : 0;
    root->black_height = offset + std::max(nodeBlackHeight(root->gauche), nodeBlackHeight(root->droite));
}

template<typename K, typename V>
size_t RedBlackTree<K, V>::nodeBlackHeight(SousArbre root) const {
    if (root == nil) return 0;
    return root->black_height;
}

template<typename K, typename V>
bool RedBlackTree<K, V>::auxBlackRuleIsEnforced(SousArbre root) const {
    if (root == nil) return true;
    return nodeBlackHeight(root->droite) == nodeBlackHeight(root->gauche) && auxBlackRuleIsEnforced(root->droite) &&
           auxBlackRuleIsEnforced(root->gauche);
}

template<typename K, typename V>
bool RedBlackTree<K, V>::blackRuleIsEnforced() const {
    updateBlackHeights(racine);
    return auxBlackRuleIsEnforced(racine);
}

template<typename K, typename V>
bool RedBlackTree<K, V>::redRuleIsEnforced() const {
    return auxRedRuleIsEnforced(racine);
}

template<typename K, typename V>
bool RedBlackTree<K, V>::auxRedRuleIsEnforced(SousArbre root) const {
    if (root == nil) return true;
    if (root->color == BLACK || (root->color != root->gauche->color && root->color != root->droite->color))
        return auxRedRuleIsEnforced(root->gauche) && auxRedRuleIsEnforced(root->droite);
    return false;
}

template<typename K, typename V>
bool RedBlackTree<K, V>::invariant() const {

    if (!treeIsOrdered()) return false ;
    if (!blackRuleIsEnforced()) return false ;
    if (!redRuleIsEnforced()) return false ;

    return racine->color == BLACK;
}


#endif //REDBLACK_TREE_H
