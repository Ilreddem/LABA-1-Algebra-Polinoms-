#include <gtest/gtest.h>
#include "RedBlackTree.h"

// Тест на создание дерева
TEST(RedBlackTreeTest, CanCreateEmptyTree) {
    RedBlackTree tree;
    // Пустое дерево должно иметь корень равным nullptr
    // Но после вставки узла — корень уже будет существовать
    tree.insert(10);
    ASSERT_NE(tree.root, nullptr);
    EXPECT_EQ(tree.root->data, 10);
    EXPECT_EQ(tree.root->color, BLACK); // Корень всегда должен быть черным
}

// Тест на корректную вставку одного узла
TEST(RedBlackTreeTest, InsertSingleNode) {
    RedBlackTree tree;
    tree.insert(25);
    EXPECT_EQ(tree.root->data, 25);
    EXPECT_EQ(tree.root->color, BLACK);
}

// Тест на правильный цвет детей после вставки нескольких узлов
TEST(RedBlackTreeTest, InsertMultipleNodesColorsCheck) {
    RedBlackTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30); // Должно вызвать поворот и перекраску

    EXPECT_EQ(tree.root->data, 20);
    EXPECT_EQ(tree.root->color, BLACK);
    EXPECT_EQ(tree.root->left->data, 10);
    EXPECT_EQ(tree.root->left->color, RED);
    EXPECT_EQ(tree.root->right->data, 30);
    EXPECT_EQ(tree.root->right->color, RED);
}

// Тест на обход дерева (inorder должен выдавать отсортированную последовательность)
TEST(RedBlackTreeTest, InorderTraversalOutputsSorted) {
    RedBlackTree tree;
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf()); // Перехват cout

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    tree.inorder();

    std::cout.rdbuf(oldCout); // Возврат стандартного вывода

    std::string output = buffer.str();
    EXPECT_EQ(output, "20 30 40 50 60 70 80 ");
}
enable_testing()
add_executable(RBTreeTests red_black_tree_test.cpp RedBlackTree.cpp)
target_link_libraries(RBTreeTests gtest gtest_main)
add_test(NAME RBTreeTests COMMAND RBTreeTests)
