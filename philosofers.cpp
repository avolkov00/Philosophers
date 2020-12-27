#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

mutex forks[5];

mutex cout_mutex;
void print_msg(const string& msg)
{
	cout_mutex.lock();
	cout << msg << flush;
	cout_mutex.unlock();
}


void Phil(size_t idx, const string& id)
{
  mutex& left_fork  = forks[ idx==0 ? 4 : idx-1 ];
  mutex& right_fork = forks[ idx ];
  int time_passed;
  ostringstream ostr;
  int hunger=0;
	while(true)
	{
		
	    time_passed = rand() % 10;
		this_thread::sleep_for(chrono::seconds(time_passed));
		hunger += time_passed;
		ostringstream ostr;
		ostr << id << " was thinking for " << time_passed << " seconds. His hunger is "<< hunger << endl;
		print_msg(ostr.str());
		ostr.str("");;

		if ((hunger >= 10) && (hunger <= 30)) {
			ostr << id << " is hungry!:" << hunger << endl;
			print_msg(ostr.str());
			ostr.str("");;

			if (left_fork.try_lock()) {

				ostringstream ostr;
				ostr << id << " got L Fork:" << hunger << endl;
				print_msg(ostr.str());
				ostr.str("");;

				if (right_fork.try_lock()) {

					ostr << id << " got R Fork:" << hunger << endl;
					print_msg(ostr.str());
					ostr.str("");;

					ostr << id << " can eat!:" << hunger << endl;
					print_msg(ostr.str());
					ostr.str("");;

					this_thread::sleep_for(chrono::seconds(1));
					hunger = 0;
					ostr << id << " is not hungry anymore:" << hunger << endl;
					print_msg(ostr.str());
					ostr.str("");;

					right_fork.unlock();
					left_fork.unlock();

				}
				else {
					ostr << id << " can't eat!:" << hunger << endl;
					print_msg(ostr.str());
					ostr.str("");;

					left_fork.unlock();
				}
			}
		}
		else if (hunger > 30) {
			ostr << id << "is VERY hungry!:" << hunger << endl;
			print_msg(ostr.str());
			ostr.str("");;
			left_fork.lock();

			ostr << id << " got L Fork:" << hunger << endl;
			print_msg(ostr.str());
			ostr.str("");;

			right_fork.lock();

			ostr << id << " got R Fork:" << hunger << endl;
			print_msg(ostr.str());
			ostr.str("");;

			ostr << id << " can eat!:" << hunger << endl;
			print_msg(ostr.str());
			ostr.str("");;

			this_thread::sleep_for(chrono::seconds(1));
			hunger = 0;
			ostr << id << " is not hungry anymore:" << hunger << endl;
			print_msg(ostr.str());
			ostr.str("");;

			right_fork.unlock();
			left_fork.unlock();
			
		}
	}
}



int main()
{
	srand(time(0));
	vector<thread> philosophers;
	philosophers.push_back(thread(Phil,0,"Socrat"));
	philosophers.push_back(thread(Phil,1,"Platon"));
	philosophers.push_back(thread(Phil,2,"Decart"));
	philosophers.push_back(thread(Phil,3,"Popper"));
	philosophers.push_back(thread(Phil,4,"Aristotel"));

	for(auto &t : philosophers) t.join();
	
	return 0;


}