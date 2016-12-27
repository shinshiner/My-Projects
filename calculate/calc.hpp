//计算器类，计算连续乘方时规则和正常思维可能不一样，尽量多加括号
class calc {
	char *expression;	//要计算的中缀表达式
	enum token { OPAREN, ADD, SUB, MULTI, DIV, EXP, CPAREN, VALUE, EOL };

	void BinaryOp(token op, seqStack<int> &dataStack);
	token getOp(int &value);

public:
	calc(char *s) { expression = new char[strlen(s) + 1]; strcpy(expression, s); }
	~calc() { delete expression; }
	int result();	//计算表达式结果
};

//-----private-----//

void calc::BinaryOp(token op, seqStack<int> &dataStack) {
	int num1, num2;

	if (dataStack.isEmpty()) { cout << "缺操作数！"; exit(1); }
	else num2 = dataStack.pop();
	if (dataStack.isEmpty()) { cout << "缺操作数！"; exit(1); }
	else num1 = dataStack.pop();

	switch (op){
		case calc::ADD:dataStack.push(num1 + num2);	break;
		case calc::SUB:dataStack.push(num1 - num2);	break;
		case calc::MULTI:dataStack.push(num1 * num2);	break;
		case calc::DIV:dataStack.push(num1 / num2);	break;
		case calc::EXP:dataStack.push(pow(num1, num2));
	}
}

calc::token calc::getOp(int &value) {
	while (*expression) {
		//跳过表达式中的空格
		while (*expression&&*expression == ' ') ++expression;

		//处理运算数
		if (*expression <= '9'&&*expression >= '0') {
			value = 0;
			while (*expression >= '0'&&*expression <= '9') {
				value = value * 10 + *expression - '0';
				++expression;
			}
			return VALUE;
		}

		switch (*expression) {
			case'(':++expression; return OPAREN;
			case')':++expression; return CPAREN;
			case'+':++expression; return ADD;
			case'-':++expression; return SUB;
			case'*':++expression; return MULTI;
			case'/':++expression; return DIV;
			case'^':++expression; return EXP;
		}
	}
	return EOL;
}

//-----public-----//

int calc::result() {
	token lastOp, topOp;
	int result_value, currentValue;
	seqStack<token> opStack;	//运算符栈
	seqStack<int> dataStack;	//运算数栈
	char *str = expression;

	//从头到尾扫描表达式
	while (true) {
		lastOp = getOp(currentValue);
		if (lastOp == EOL) break;
		switch (lastOp) {
			case VALUE:dataStack.push(currentValue); break;
			case CPAREN:	//开闭括号之间的所有运算都可进行
				while (!opStack.isEmpty() && (topOp = opStack.pop()) != OPAREN)
					BinaryOp(topOp, dataStack);
				if (topOp != OPAREN) cout << "缺左括号\n";
				break;
			case OPAREN:opStack.push(OPAREN); break;
			case EXP:opStack.push(EXP); break;
			case MULTI:case DIV:
				while (!opStack.isEmpty() && opStack.top() >= MULTI)
					BinaryOp(opStack.pop(), dataStack);
				opStack.push(lastOp);
				break;
			case ADD:case SUB:
				while (!opStack.isEmpty() && opStack.top() != OPAREN)
					BinaryOp(opStack.pop(), dataStack);
				opStack.push(lastOp);
		}
	}

	while (!opStack.isEmpty()) BinaryOp(opStack.pop(), dataStack);
	if (dataStack.isEmpty()) { cout << "无结果\n"; return 0; }
	result_value = dataStack.pop();
	if (!dataStack.isEmpty()) { cout << "缺操作符\n"; return 0; }

	expression = str;
	return result_value;
}
