#!/usr/bin/env python3

from time import sleep
from threading import Thread
from multiprocessing import Process
import asyncio  # Import for asynchronous tasks.

def work_sleep():
    for i in range(10000): 
        sleep(0)  # Synchronous pause for threads and processes.

def work_i():
    lst = [i * i for i in range(1000000)]  # Compute squares.

async def async_work():
    lst = [i * i for i in range(1000000)]  # Asynchronous computation of squares.

async def async_work_sleep():
    for i in range(10000): 
        await asyncio.sleep(0)  # Asynchronous pause.

def use_multiprocessing():
    processes = []
    for _ in range(100):  # Launch 100 processes.
        proc = Process(target=work_sleep)
        processes.append(proc)
        proc.start()

    for proc in processes: 
        proc.join()

def use_threading():
    threads = []
    for _ in range(100):  # Launch 100 threads.
        thread = Thread(target=work_sleep)
        threads.append(thread)
        thread.start()

    for thread in threads: 
        thread.join()

async def use_asyncio():
    tasks = []
    for _ in range(100):  # Launch 100 asynchronous tasks.
        task = asyncio.create_task(async_work_sleep())
        tasks.append(task)

    await asyncio.gather(*tasks)

if __name__ == "__main__":
    use_multiprocessing()
    use_threading()
    asyncio.run(use_asyncio())
