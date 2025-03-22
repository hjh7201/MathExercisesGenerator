#define _CRT_SECURE_NO_WARNINGS // 禁用安全警告
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unordered_set> // 引入哈希表

// 计算最大公约数（GCD）
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 生成随机数
int randomInt(int max) {
    return rand() % max;
}

// 生成一个四则运算题目
void generateExpression(int max, char* expression, char* answer) {
    int num1 = randomInt(max) + 1; // 避免除数为0
    int num2 = randomInt(max) + 1;
    int op = randomInt(4); // 0: +, 1: -, 2: *, 3: /

    char operatorChar;
    switch (op) {
    case 0: operatorChar = '+'; break;
    case 1: operatorChar = '-'; break;
    case 2: operatorChar = '*'; break;
    case 3: operatorChar = '/'; break; // 使用 / 代替 ÷
    default: operatorChar = '+'; break;
    }

    // 如果是减法，确保 num1 >= num2
    if (operatorChar == '-' && num1 < num2) {
        int temp = num1;
        num1 = num2;
        num2 = temp;
    }

    // 生成题目
    sprintf(expression, "%d %c %d =", num1, operatorChar, num2);

    // 计算答案
    if (operatorChar == '+') {
        sprintf(answer, "%d", num1 + num2);
    }
    else if (operatorChar == '-') {
        sprintf(answer, "%d", num1 - num2);
    }
    else if (operatorChar == '*') {
        sprintf(answer, "%d", num1 * num2);
    }
    else if (operatorChar == '/') {
        // 化简分数
        int commonDivisor = gcd(num1, num2);
        int numerator = num1 / commonDivisor;
        int denominator = num2 / commonDivisor;

        if (denominator == 1) {
            sprintf(answer, "%d", numerator); // 结果为整数
        }
        else {
            sprintf(answer, "%d/%d", numerator, denominator); // 结果为真分数
        }
    }
}

// 生成题目并写入文件
void generateExercises(int n, int r, const char* exerciseFile, const char* answerFile) {
    FILE* exFile = fopen(exerciseFile, "w");
    FILE* ansFile = fopen(answerFile, "w");

    if (!exFile || !ansFile) {
        printf("Error: Could not open file.\n");
        return;
    }

    std::unordered_set<std::string> generatedExpressions; // 存储已生成的题目

    for (int i = 1; i <= n; i++) {
        char expression[100];
        char answer[100];
        while (true) {
            generateExpression(r, expression, answer);
            if (generatedExpressions.find(expression) == generatedExpressions.end()) {
                generatedExpressions.insert(expression); // 添加到集合
                break;
            }
        }

        fprintf(exFile, "%d. %s\n", i, expression);
        fprintf(ansFile, "%d. %s\n", i, answer);
    }

    fclose(exFile);
    fclose(ansFile);
}

// 批改题目并输出结果
void gradeExercises(const char* exerciseFile, const char* answerFile, const char* gradeFile) {
    FILE* exFile = fopen(exerciseFile, "r");
    FILE* ansFile = fopen(answerFile, "r");
    FILE* gradeFilePtr = fopen(gradeFile, "w");

    if (!exFile || !ansFile || !gradeFilePtr) {
        printf("Error: Could not open file.\n");
        return;
    }

    int correctCount = 0, wrongCount = 0;
    int correctNumbers[10000], wrongNumbers[10000];
    int index = 0;

    char exLine[100], ansLine[100];
    while (fgets(exLine, sizeof(exLine), exFile) && fgets(ansLine, sizeof(ansLine), ansFile)) {
        int exNum, ansNum;
        int num1, num2;
        char operatorChar;

        sscanf(exLine, "%d. %d %c %d", &exNum, &num1, &operatorChar, &num2);

        char expectedAnswer[100];
        sscanf(ansLine, "%d. %s", &exNum, expectedAnswer);

        char computedAnswer[100];
        if (operatorChar == '+') {
            sprintf(computedAnswer, "%d", num1 + num2);
        }
        else if (operatorChar == '-') {
            sprintf(computedAnswer, "%d", num1 - num2);
        }
        else if (operatorChar == '*') {
            sprintf(computedAnswer, "%d", num1 * num2);
        }
        else if (operatorChar == '/') {
            int commonDivisor = gcd(num1, num2);
            int numerator = num1 / commonDivisor;
            int denominator = num2 / commonDivisor;

            if (denominator == 1) {
                sprintf(computedAnswer, "%d", numerator);
            }
            else {
                sprintf(computedAnswer, "%d/%d", numerator, denominator);
            }
        }

        if (strcmp(computedAnswer, expectedAnswer) == 0) {
            correctNumbers[correctCount++] = exNum;
        }
        else {
            wrongNumbers[wrongCount++] = exNum;
        }
    }

    fprintf(gradeFilePtr, "Correct: %d (", correctCount);
    for (int i = 0; i < correctCount; i++) {
        fprintf(gradeFilePtr, "%d", correctNumbers[i]);
        if (i < correctCount - 1) fprintf(gradeFilePtr, ", ");
    }
    fprintf(gradeFilePtr, ")\n");

    fprintf(gradeFilePtr, "Wrong: %d (", wrongCount);
    for (int i = 0; i < wrongCount; i++) {
        fprintf(gradeFilePtr, "%d", wrongNumbers[i]);
        if (i < wrongCount - 1) fprintf(gradeFilePtr, ", ");
    }
    fprintf(gradeFilePtr, ")\n");

    fclose(exFile);
    fclose(ansFile);
    fclose(gradeFilePtr);
}


int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));

    if (argc < 2) {
        printf("Usage:\n");
        printf("  Generate exercises: Myapp.exe -n <number> -r <range>\n");
        printf("  Grade exercises: Myapp.exe -e <exercisefile> -a <answerfile>\n");
        return 1;
    }

    if (strcmp(argv[1], "-n") == 0 && argc == 5) {
        int n = atoi(argv[2]);
        int r = atoi(argv[4]);
        generateExercises(n, r, "Exercises.txt", "Answers.txt");
    }
    else if (strcmp(argv[1], "-e") == 0 && argc == 5) {
        gradeExercises(argv[2], argv[4], "Grade.txt");
    }
    else {
        printf("Invalid arguments.\n");
        return 1;
    }

    return 0;
}