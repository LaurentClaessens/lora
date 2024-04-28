import time
from typing import TYPE_CHECKING

import threading
from threading import Thread
from queue import Queue

from src.stubs import LoraJob
from src.exceptions import ClosedManager

if TYPE_CHECKING:
    from src.options import Options


class JobsManager:

    def __init__(self, options: 'Options'):
        """Initialize."""
        self.options = options
        self.jobs: Queue[LoraJob] = Queue()
        self.open: bool = True
        self.finished: bool = False
        self.thread = Thread(target=self.run)

    def start(self):
        """Start the jobs."""
        self.thread.start()

    def append(self, job: LoraJob):
        """Append a job to the list if needed."""
        if not self.open:
            raise ClosedManager("Try to append task to a closed manager")
        if not job.has_to_to():
            return
        self.jobs.put(job)

    def stop(self):
        self.open = False
        print(f"The jobs manager is now closed. ({self.qsize()})")

    def has_jobs(self):
        """Say if there are some more jobs."""
        return self.qsize() != 0

    def do_one_loop(self):
        """Work until the job list is empty."""
        while self.has_jobs():
            self.check_threads()
            job = self.jobs.get()
            print("queue: ", self.jobs.qsize(), end="\r")
            job.run()

    def are_all_threads_ok(self):
        """Say if all the threads are ok."""
        if not threading.main_thread().is_alive():
            return False
        if not self.thread.is_alive():
            return False
        return True

    def emergency_stop(self):
        """Make an emergency stop."""
        print("making an emergency stop.")
        self.stop()
        with self.jobs.mutex:
            self.jobs.queue.clear()
        self.close_threads()

    def close_threads(self):
        """Close the threads. Only finishes the created jobs."""
        self.stop()
        self.wait_finished()

    def check_threads(self):
        """
        Check that the main thread is alive.

        If not, empty the job list as fast as possible.
        """
        if self.are_all_threads_ok():
            return
        print("Some threads hase problems. I stop.")
        self.emergency_stop()

    def qsize(self):
        """Return the (approximate) size of the queue."""
        return self.jobs.qsize()

    def wait_finished(self):
        """Wait that all the jobs are done."""
        print(f"Waiting that all the jobs are done. ({self.qsize()})")
        while not self.finished:
            time.sleep(1)

    def run(self):
        while self.open:
            self.check_threads()
            self.do_one_loop()
            print("waiting for new jobs...", end="\r")
            time.sleep(1)
        print("manager is closed. One last loop.")
        self.do_one_loop()
        print("manager finished.")
        self.finished = True
