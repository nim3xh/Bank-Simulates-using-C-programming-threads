for Compile: gcc -pthread cq.c -o cq
for run    : ./cq 20 1 1 1 1

This is a C program that simulates a bank with multiple tellers and customers using threads. The program is intended to be run from the command line and accepts five arguments: the length of the customer queue, the periodic time for the customer to arrive, the time duration to serve withdrawal, the time duration to serve deposit, and the time duration to serve information.

The program defines two structures: "cus" and "tel", for customers and tellers, respectively. "cus" structure has three fields: "cusID", which is the customer ID, "ServiceType", which is the type of service the customer wants ("W" for withdrawal, "D" for deposit, and "I" for information), and "arr_time", which is the time of arrival of the customer. "tel" structure has two fields: "tellerID", which is the teller ID, and "thread", which is the thread that the teller is running on.

The program defines some global variables, such as the size of the customer queue, the time durations, and the queue itself. It also defines some file pointers for reading and writing data, as well as some variables to keep track of the number of customers served by each teller and the total number of customers.

The program defines several functions, including "enQueue" and "deQueue" for adding and removing customers from the queue, respectively, "customer" for simulating customer behavior, "teller" for simulating teller behavior, "ServedCustomers" for outputting the number of customers served by each teller, and "termination" for gracefully terminating the program.

In the "main" function, the program checks the number of command line arguments and initializes some variables. It then creates threads for the customer and each teller using the "pthread_create" function and waits for the customer thread to finish using the "pthread_join" function. Finally, it calls the "termination" function.
