#include <base/printf.h>
#include <base/thread.h>
#include <base/env.h>
#include <cpu_session/connection.h>


using namespace Genode;



struct Cpu_helper : Thread<0x2000>
{
	Cpu_helper(const char * name, Cpu_session * cpu)
	: Thread<0x2000>(name, cpu) { }

	void entry()
	{
		printf("%s : _cpu_session=0x%p env()->cpu_session()=0x%p\n", _context->name, _cpu_session, env()->cpu_session());
	}
};


static void test_cpu_session()
{
	printf("running '%s'\n", __func__);

	Cpu_helper thread0("prio high  ", env()->cpu_session());
	thread0.start();
	thread0.join();

	printf("create thread prio middle\n");

	Cpu_connection con1("prio middle", Cpu_session::PRIORITY_LIMIT / 4);
	Cpu_helper thread1("prio middle", &con1);
	thread1.start();
	thread1.join();

	printf("create thread prio low\n");

	Cpu_connection con2("prio low", Cpu_session::PRIORITY_LIMIT / 2);
	Cpu_helper thread2("prio low   ", &con2);
	thread2.start();
	thread2.join();
}

static void test_cpu_session_edf_fp()
{

	env()->cpu_session()->create_thread(10, "test", 0);
	env()->cpu_session()->create_fp_edf_thread(10, "test2", 0, 1, 0);

	/*
	//Create fp_thread
	Cpu_connection con1("fp middle", 64, 0);
	Cpu_helper thread1 ("fp middle", &con1);
	thread1.start();
	thread1.join();
	*/
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
