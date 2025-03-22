| 作业gitHub     | [作业链接](https://github.com/hjh7201/MathExercisesGenerator)     |      
| ---- | ---- | 
|  这个作业属于哪个课程    |  软件工程    |     
|   这个作业要求在哪里   |  [作业要求](https://edu.cnblogs.com/campus/gdgy/SoftwareEngineeringClassof2023/homework/13326)    | 
|  这个作业的目标    | 合作完成一次项目    |    

##1.PSP表格
|  PSP2.1    |  Personal Software Process Stages    |   预估耗时（分钟）   |实际耗时（分钟）|
| ---- | ---- | ---- |-|
|  Planning    |   计划   |30      |40|
|    Estimate  |   估计这个任务需要多少时间   | 1600     |1500|
|    Development  |  开发    |  200    |300|
|    Analysis  |  需求分析 (包括学习新技术)    | 300     |500|
|    Design Spec  | 生成设计文档    |  30    |30|
|    Design Review  |  设计复审    | 40     |30|
|   Coding Standard   |   代码规范 (为目前的开发制定合适的规范)   |30      |40|
| Design     |  具体设计    | 60     |50|
|    Coding  |   具体编码   |  180    |180|
|   Code Review   | 代码复审     |    60  |60|
|   Test   |  测试（自我测试，修改代码，提交修改）    |300      |500|
|  Reporting    | 报告     |   100   |120|
|   Test Repor   |   测试报告   |   40   |50|
| Size Measurement     |  计算工作量    | 50     |60|
|  Postmortem & Process Improvement Plan    |  事后总结, 并提出过程改进计划    |    50  |60|
|      |  合计    |  3070    |3340|
---
 ##2. 效能分析
#### **改进思路**
1. **优化随机数生成**：
   - 初始版本中，随机数生成效率较低，改进后使用更高效的随机数生成算法。
2. **减少文件读写次数**：
   - 将多次文件读写操作合并为单次读写，减少 I/O 开销。
3. **优化字符串处理**：
   - 使用更高效的字符串拼接方法，减少内存分配次数。

#### **性能分析图**
以下是性能分析工具的截图
![](https://img2024.cnblogs.com/blog/3610099/202503/3610099-20250322172038656-732678073.png)
## **3. 设计实现过程**
#### **代码组织**
- **模块划分**：
  1. **文件读取模块**：负责读取题目和答案文件。
  2. **题目生成模块**：生成四则运算题目。
  3. **答案计算模块**：计算题目答案。
  4. **批改模块**：对比答案并统计结果。

#### **关键函数**
- `generateExpression`：生成一个四则运算题目。
- `calculateAnswer`：计算题目答案。
- `gradeExercises`：批改题目并输出结果。

#### **流程图**
以下是 `generateExpression` 函数的流程图：
```plaintext
开始
  ↓
生成随机数 num1 和 num2
  ↓
生成随机运算符 op
  ↓
如果是减法，确保 num1 >= num2
  ↓
生成题目字符串
  ↓
计算答案
  ↓
返回题目和答案
  ↓
结束
```

---

## **4. 代码说明**
#### **关键代码**
```c
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
    } else if (operatorChar == '-') {
        sprintf(answer, "%d", num1 - num2);
    } else if (operatorChar == '*') {
        sprintf(answer, "%d", num1 * num2);
    } else if (operatorChar == '/') {
        // 化简分数
        int commonDivisor = gcd(num1, num2);
        int numerator = num1 / commonDivisor;
        int denominator = num2 / commonDivisor;

        if (denominator == 1) {
            sprintf(answer, "%d", numerator); // 结果为整数
        } else {
            sprintf(answer, "%d/%d", numerator, denominator); // 结果为真分数
        }
    }
}
```

#### **代码思路**
- **随机数生成**：使用 `rand()` 生成随机数，确保数值在指定范围内。
- **运算符选择**：通过 `switch` 语句随机选择运算符。
- **答案计算**：根据运算符计算答案，并将分数化简为最简形式。
---

## **5. 测试运行**
#### **测试用例**
以下是 10 个测试用例及其预期结果：
| **题目**       | **预期答案** |
|----------------|--------------|
| 5 + 3 =        | 8            |
| 7 - 2 =        | 5            |
| 4 * 6 =        | 24           |
| 9 / 3 =        | 3            |
| 10 - 7 =       | 3            |
| 2 + 8 =        | 10           |
| 5 * 4 =        | 20           |
| 6 / 2 =        | 3            |
| 3 + 6 =        | 9            |
| 8 - 4 =        | 4            |
#### **测试结果**
- 所有测试用例均通过，程序功能正确。

### **7. 项目小结**
#### **成败得失**
- **成功之处**：
  - 实现了题目生成、答案计算和批改功能。
  - 代码结构清晰，易于扩展。
- **不足之处**：
  - 初始版本性能较低，经过优化后有所改善。
  - 部分边界情况（如除数为 0）未完全处理。

#### **结对感受**
-起初，面对合作，双方还有些拘谨，担心合作不顺畅。但很快，就找到了默契，再遇到瓶颈时，我们共同商讨，解决了代码bug，速度等问题，简洁，收获颇丰
合作不仅在代码优化方面提供了很多宝贵建议，也在设计文档和测试用例方面做得非常细致，学到了很多。
