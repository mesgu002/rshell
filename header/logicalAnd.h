#ifndef LOGICALAND_H
#define LOGICALAND_H
class LogicalAnd : public Connector {
	public:
		LogicalAnd();
		~LogicalAnd();
		void run(bool &previous, vector<string> &commands);
};

#endif // LOGICALAND_H