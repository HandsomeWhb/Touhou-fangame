#pragma once
#include "page.h"
class Score {
public:
	int year;
	int month;
	int day;
	int hour;
	int minute;
	std::string name;
	int score;
	Score();
	Score(int y,int m,int d,int h,int min,std::string name,int score);
};
class Score_manager {
public:
	std::string file_path;
	std::vector<Score> scores;
	std::vector<Score> load_all_score();
	void add_new_score(std::string name,int score);
	int get_highest_score();
	void save_score();
	Score_manager(std::string file_path);

};
class Score_center {
public:
	static std::string file_path;
	static std::vector<Score_manager*> score_manager_ptrs;
	static Score_manager* search(std::string file_path);
	static void init(std::string file_path);
	static void load_all_score_manager();
	static void add_score_manager(Score_manager* score_manager_ptr);
};