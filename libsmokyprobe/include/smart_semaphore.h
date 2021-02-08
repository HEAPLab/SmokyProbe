
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>


class smart_semaphore
{
public:

	smart_semaphore(sem_t * s) {
		__sem = s;
		sem_wait(__sem);
	}

	virtual ~smart_semaphore() {
		sem_post(__sem);
	}

	smart_semaphore(smart_semaphore && o) {
		__sem = o.__sem;
		o.__sem = nullptr;
	}

	smart_semaphore() = delete;

	smart_semaphore(sem_t &) = delete;


	smart_semaphore & operator=(smart_semaphore & ) = delete;

	smart_semaphore & operator=(smart_semaphore && o ) {
		__sem = o.__sem;
		o.__sem = nullptr;
	}


private:

	sem_t * __sem;

};
