#include <base/printf.h>
#include <base/thread.h>
#include <base/env.h>
#include <cpu_session/connection.h>


using namespace Genode;


enum{
	ALL = 0,
	FIXED_PRIO = 1,
	EDF   = 2
};


static void create_thread_helper(char const* name, unsigned priority, unsigned deadline, unsigned cpu){
	env()->cpu_session()->create_fp_edf_thread(10, name, 0, priority, deadline, cpu);
}

static void test_cpu_session_edf_fp()
{

	Affinity::Space affinity_space = env()->cpu_session()->affinity_space();
	printf("affinity space: %d\n", affinity_space.total());

	//Set FP to core0
	env()->cpu_session()->set_sched_type(0, FIXED_PRIO);

	//Set EDF to core1
	env()->cpu_session()->set_sched_type(1, EDF);


	create_thread_helper("test1", 2, 0, 0);
	create_thread_helper("test2", 3, 0, 0);

	//Create FP-Thread on CPU1
	create_thread_helper("test10", 4, 0, 1);

	//Create illegal thread
	//create_thread_helper("test3", 2, 2);


	//create_thread_helper("test5", 0, 2, 0);

	//Create edf threads on cpu1
	create_thread_helper("test6", 0, 4, 1);
	create_thread_helper("test7", 0, 3, 1);
	create_thread_helper("test8", 0, 2, 1);

	//Create edf threads on cpu0
	create_thread_helper("test4", 0, 1, 0);

	//Create FP_thread on cpu1
	create_thread_helper("test9", 50, 0, 1);

}

int main()
{
	printf("--- thread test started ---\n");

	try {
		
		//test_cpu_session();
		test_cpu_session_edf_fp();
	} catch (int error) {
		return error;
	}
}
