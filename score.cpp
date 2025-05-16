#include "score.h"
using namespace std;
using namespace sf;
using namespace filesystem;
Score::Score(){}
Score::Score(int y, int m, int d, int h, int min, std::string name, int score):year(y),month(m),
day(d),hour(h),minute(min),name(name),score(score){}
Score_manager::Score_manager(std::string file_path):file_path(file_path){}

string Score_center::file_path;
vector<Score_manager*> Score_center::score_manager_ptrs;
void Score_center::init(std::string file_path){
	Score_center::file_path = file_path;
	load_all_score_manager();
}
void Score_center::load_all_score_manager() {
	string relative_path = "./" + file_path;
	cout << "开始加载分数" << " 文件夹:" << relative_path << endl;
	if (!exists(relative_path)) {
		cout << "路径不存在：" << relative_path << endl;
		return;
	}
	if (!is_directory(relative_path)) {
		cout << "不是一个目录：" << relative_path << endl;
		return;
	}
	vector<string> fileNames;
	// 遍历文件夹
	Score_manager* res; 
	for (const auto& entry : directory_iterator(relative_path)) {
		if (entry.is_regular_file()) {
			// 获取文件名（不带路径）
			string filename = entry.path().filename().string();
			fileNames.push_back(filename);
			cout << "加载文件: " << filename << endl;
			res = new Score_manager(file_path + filename);
			add_score_manager(res);
		}
	}
	// 文件名存储在 vector 中，可以进一步处理
	cout << "共加载 " << fileNames.size() << " 个文件。" << endl;
	cout << "分数配置加载完毕" << endl;
	return;

}
void Score_center::add_score_manager(Score_manager* score_manager_ptr) {
	score_manager_ptr->load_all_score();
	score_manager_ptrs.push_back(score_manager_ptr);
}
Score_manager* Score_center::search(std::string file_path) {
	for (auto it = score_manager_ptrs.begin(); it != score_manager_ptrs.end(); it++) {
		if ((*it)->file_path == Score_center::file_path + file_path) {
			return *it;
		}
	}
	return nullptr;
}
void Score_manager::add_new_score(string name, int score) {
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	Score new_score(1900 + ltm.tm_year, 1 + ltm.tm_mon, ltm.tm_mday, ltm.tm_hour, ltm.tm_min, name, score);
	scores.push_back(new_score);
	std::sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) {
		return a.score > b.score;
		});
	if (scores.size() > 10)
		scores.resize(10);
	save_score(); 
}
std::vector<Score> Score_manager::load_all_score() {
	std::vector<Score> result;
	std::ifstream file(file_path);
	if (!file.is_open()) return result;
	int y, m, d, h, mi, s;
	std::string name;
	while (file >> y >> m >> d >> h >> mi >> name >> s) {
		result.emplace_back(y, m, d, h, mi, name, s);
	}
	scores = result;
	return result;
}
void Score_manager::save_score() {
	std::ofstream file(file_path, std::ios::trunc);
	for (const auto& sc : scores) {
		file << sc.year << ' ' << sc.month << ' ' << sc.day << ' ' << sc.hour << ' '
			<< sc.minute << ' ' << sc.name << ' ' << sc.score << '\n';
	}
}
int Score_manager::get_highest_score()  {
	if (scores.empty()) return 0;
	return scores.front().score;
}