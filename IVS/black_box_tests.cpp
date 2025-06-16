//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     BORIS NICOLAS DRÁB <xdrabbo00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author BORIS NICOLAS DRÁB
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//




class EmptyTree : public ::testing::Test 
{
protected:
    BinaryTree tree;
    Node_t* tree_root;

    virtual void SetUp() override {
        tree_root = tree.GetRoot();
    }
};

class NonEmptyTree : public ::testing::Test 
{
protected:
    virtual void SetUp() override {
        int values[] = {10, 85, 15, 71, 23, 42, 44, 68, 65, 20, 74, 40, 90, 55};
        for (auto value : values) {
            tree.InsertNode(value);
        }
    }
    BinaryTree tree;
};


class Axiom : public ::testing::Test
{
protected: 
    virtual void SetUp() override {
        for (unsigned index = 0; index < 50; index++) {
            tree.InsertNode(10 + rand() % 42);
        }
    }
    BinaryTree tree;
    Node_t* tree_root;
};




TEST_F (EmptyTree, InsertNode) {
    ASSERT_EQ(tree_root, nullptr);

    auto result = tree.InsertNode(42);

    EXPECT_TRUE(result.first);

    EXPECT_EQ(result.second->key, 42);

    ASSERT_NE(result.second, nullptr);
    EXPECT_NE(result.second->pRight, nullptr);
    EXPECT_NE(result.second->pLeft, nullptr);

    EXPECT_NE(result.second->pRight->pParent, nullptr);
    EXPECT_NE(result.second->pLeft->pParent, nullptr);


    auto result2 = tree.InsertNode(42);
    EXPECT_FALSE(result2.first);
    EXPECT_EQ(result2.second, result.second);
}

TEST_F (EmptyTree, DeleteNode) {
    ASSERT_EQ(tree_root, nullptr);
    tree.DeleteNode(8);

    EXPECT_FALSE(tree.FindNode(8));
}

TEST_F (EmptyTree, FindNode) {
    ASSERT_EQ(tree_root, nullptr);

    EXPECT_EQ(tree.FindNode(4), nullptr);
}




TEST_F (NonEmptyTree, InsertNode_DuplicateKey) {
    tree.InsertNode(3);
    tree.InsertNode(2);
    tree.InsertNode(5);

    auto result = tree.InsertNode(3);
    EXPECT_FALSE(result.first);

    EXPECT_EQ(result.second->key, 3);

    EXPECT_TRUE(tree.FindNode(3));
    EXPECT_TRUE(tree.FindNode(2));
    EXPECT_TRUE(tree.FindNode(5));
}

TEST_F (NonEmptyTree, InsertNode_DifferentKey) {
    int value = 11;

    auto inserted_node = tree.InsertNode(value);

    EXPECT_TRUE(inserted_node.first);
    ASSERT_NE(inserted_node.second, nullptr);

    auto found_node = tree.FindNode(value);

    EXPECT_EQ(found_node, inserted_node.second);
    EXPECT_EQ(found_node->key, value);
}

TEST_F(NonEmptyTree, FindNode_Found) {
    tree.InsertNode(5);

    int value = 5;

    auto found = tree.FindNode(value);

    EXPECT_TRUE(found != nullptr);
}

TEST_F(NonEmptyTree, FindNode_NotFound) {
    int value = 3;

    auto not_found = tree.FindNode(value);

    EXPECT_TRUE(not_found == nullptr);
}

TEST_F (NonEmptyTree, DeleteNode) {
    tree.InsertNode(3);
    tree.InsertNode(2);
    tree.InsertNode(5);

    tree.DeleteNode(2);

    EXPECT_FALSE(tree.FindNode(2));

    EXPECT_TRUE(tree.FindNode(3));
    EXPECT_TRUE(tree.FindNode(5));
}




TEST_F (Axiom, Axiom1) {
    std::vector<BinaryTree::Node_t*> leafNodes;
    tree.GetLeafNodes(leafNodes);

    for (auto index = 0; index < leafNodes.size(); index++) {
        EXPECT_EQ(leafNodes[index]->color, tree.BLACK);
    }
}

TEST_F (Axiom, Axiom2) {
    std::vector<Node_t*> leaf;
    tree.GetLeafNodes(leaf);

    for (auto index = 0; index < leaf.size(); index++) {
        if (leaf[index]->color == RED){
            if (leaf[index]->pRight != nullptr) {
                leaf[index]->pRight->color == BLACK;
                EXPECT_EQ(leaf[index]->pRight->color, BLACK);
            }

            if (leaf[index]->pLeft != nullptr) {
                leaf[index]->pLeft->color == BLACK;
                EXPECT_EQ(leaf[index]->pLeft->color, BLACK);
            }
        }
    }

}

TEST_F (Axiom, Axiom3) {
    std::vector<Node_t*> leafs;
    tree.GetLeafNodes(leafs);
    
    int max_count_par = 0;
    bool mark = true;

    for (unsigned index = 0; index < leafs.size(); index++)  {

    unsigned count_par = 0;
    while (leafs.at(index)->pParent != nullptr) {
        if (leafs.at(index)->pParent->color == BLACK) {
            count_par++;
        }
        
    }
    if (mark) {
        max_count_par = count_par;
        mark = false;
    }

    EXPECT_EQ(count_par, max_count_par);

    }
}




/*** Konec souboru black_box_tests.cpp ***/
