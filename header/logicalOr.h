#ifndef LOGICALOR_H
#define LOGICALOR_H
class LogicalOr : public Connector {
	public:
		LogicalOr();
		~LogicalOr();
		void run(bool &previous, vector<string> &commands);
};

#endif // LOGICALOR_H