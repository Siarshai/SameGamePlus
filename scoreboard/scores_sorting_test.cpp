#include "scores_sorting.h"
#include "gtest/gtest.h"


TEST(AddToScoresTest, EmptyScoresList) {
    QVector<ScoreRecord> scores;
    add_to_scores_inplace(scores, "Test", 100, 5);
    ASSERT_EQ(scores.size(), 1);
    ASSERT_EQ(scores.at(0).name, "Test");
    ASSERT_EQ(scores.at(0).score, 100);
}


TEST(AddToScoresTest, NormalScoresListInsertFirst) {
    QVector<ScoreRecord> scores = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80)
    };
    const QVector<ScoreRecord> expected = {
            ScoreRecord("Test", 110),
            ScoreRecord("Test1", 100),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80)
    };
    add_to_scores_inplace(scores, "Test", 110, 5);
    ASSERT_EQ(scores, expected);
}


TEST(AddToScoresTest, NormalScoresListInsertMiddle) {
    QVector<ScoreRecord> scores = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80)
    };
    const QVector<ScoreRecord> expected = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test", 95),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80)
    };
    add_to_scores_inplace(scores, "Test", 95, 5);
    ASSERT_EQ(scores, expected);
}


TEST(AddToScoresTest, NormalScoresListInsertEnd) {
    QVector<ScoreRecord> scores = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80)
    };
    const QVector<ScoreRecord> expected = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80),
            ScoreRecord("Test", 75)
    };
    add_to_scores_inplace(scores, "Test", 75, 5);
    ASSERT_EQ(scores, expected);
}


TEST(AddToScoresTest, FullScoresListInsertMiddle) {
    QVector<ScoreRecord> scores = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80),
            ScoreRecord("Test4", 70),
            ScoreRecord("Test5", 60)
    };
    const QVector<ScoreRecord> expected = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test", 95),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80),
            ScoreRecord("Test4", 70)
    };
    add_to_scores_inplace(scores, "Test", 95, 5);
    ASSERT_EQ(scores, expected);
}


TEST(AddToScoresTest, FullScoresListInsertEnd) {
    QVector<ScoreRecord> scores = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80),
            ScoreRecord("Test4", 70),
            ScoreRecord("Test5", 60)
    };
    const QVector<ScoreRecord> expected = scores;
    add_to_scores_inplace(scores, "Test", 5, 5);
    ASSERT_EQ(scores, expected);
}


TEST(AddToScoresTest, IncompatibleScoresListsAreSortedAndTruncated) {
    QVector<ScoreRecord> scores = {
            ScoreRecord("Test7", 40),
            ScoreRecord("Test3", 80),
            ScoreRecord("Test4", 70),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test5", 60),
            ScoreRecord("Test1", 100),
            ScoreRecord("Test6", 50)
    };
    const QVector<ScoreRecord> expected = {
            ScoreRecord("Test1", 100),
            ScoreRecord("Test", 95),
            ScoreRecord("Test2", 90),
            ScoreRecord("Test3", 80),
            ScoreRecord("Test4", 70)
    };
    add_to_scores_inplace(scores, "Test", 95, 5);
    ASSERT_EQ(scores, expected);
}
