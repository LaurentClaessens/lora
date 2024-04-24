import time

from threading import Thread

from queue import Queue
from src.stubs import LoraJob


class JobsManager:

    def __init__(self):
        """Initialize."""
        self.jobs: Queue[LoraJob] = Queue()
        self.open: bool = True
        self.thread = Thread(target=self.run)
        self.thread.start()

    def append(self, job: LoraJob):
        """Append a job to the list if needed."""
        if not job.has_to_to():
            return
        self.jobs.put(job)

    def stop(self):
        self.open = False
        print("The jobs manager is now closed.")

    def has_jobs(self):
        """Say if there are some more jobs."""
        return self.qsize() != 0

    def do_one_loop(self):
        """Work until the job list is empty."""
        while self.has_jobs():
            job = self.jobs.get()
            print("queue: ", self.jobs.qsize(), end="\r")
            job.run()

    def qsize(self):
        """Return the (approximate) size of the queue."""
        return self.jobs.qsize()

    def run(self):
        while self.open:
            self.do_one_loop()
            print("waiting for new jobs...", end="\r")
            time.sleep(0.2)
        print("manager is closed. One last loop.")
        self.do_one_loop()
        print("manager finished.")
