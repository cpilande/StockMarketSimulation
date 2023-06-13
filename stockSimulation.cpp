/*
    Name: Carmela Coreen Pilande
    Program: The user inputs a stock file, price file, number of days
    for simulation, and amount to be transferred into the account. Using this info,
    it finds the profit per day, the remaining balance, and the amount in dividends.
*/
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>

struct stockType
{
    std::string name;
    int timesPurchased;
    double dividend;
    double price;

    stockType() // Default Constructor
    {
        name = "";
        timesPurchased = dividend = price = 0;
    }

    stockType(int price) // Constructor with inputted price
    {
        name = "";
        timesPurchased = dividend = price = 0;
    }

    // Checks if the left-hand value is less than the right-hand value
    bool operator<(const stockType &rhs)
    {
        if (price < rhs.price)
            return true;
        else
            return false;
    }

    // Copies the stockType object information
    void operator=(const stockType &rhs)
    {
        this->name = rhs.name;
        timesPurchased = rhs.timesPurchased;
        dividend = rhs.dividend;
        price = rhs.price;
    }
};

template <class Type>
class priorityQ
{
public:
    priorityQ(int cap) // Constructor
    {
        capacity = cap;
        heapArray = new stockType[cap]; // allocates heap array
        size = 0;                       // set the size variable
    };

    priorityQ(std::vector<Type> vec) // Constructor, w/ vector passed in
    {
        capacity = size = vec.size() + 1;

        for (int i = 0; i < vec.size(); i++) // assigns elements from vector to heapArray
            heapArray[i + 1] = vec[i];

        for (int i = size - 1; i > 0; i--) // buildHeap
            bubbleDown(i);
    };
    priorityQ(const priorityQ<Type> &rhs) // Copy constructor
    {
        for (int i = 0; i < size; i++)
            heapArray[i] = rhs[i];
    };
    ~priorityQ() // Destructor
    {
        delete[] heapArray;
    };
    const priorityQ<Type> &operator=(const priorityQ<Type> &rhs) // Assignment Operator
    {
        for (int i = 0; i < size; i++)
            heapArray[i] = rhs[i];
    };

    /*
    % void insert(Type e)
    % Description: This inserts a given element of any given type into the heapArray
    by incrementing the size, pushing an element to the end of the heapArray, and
    bubbling up that element
    */
    void insert(Type e)
    {
        size++;

        if (size == capacity) // if the heapArray is maxed out...
        {
            capacity *= 2; // increase the capacity

            Type *newArray; // create a new array
            newArray = new Type[capacity];

            for (int i = 1; i < size; i++) // coopy elements of the old array
                newArray[i] = heapArray[i];

            delete[] heapArray; // clear the heapArray

            heapArray = newArray; // point the heapArray to the newArray
            newArray = NULL;
        }

        heapArray[size] = e;
        bubbleUp(size);
    };

    /*
    % void deletePriority()
    % Description: Deletes the priority element, in this case, the minimum element
    */
    void deletePriority()
    {
        heapArray[1] = heapArray[size]; // Sets the root element to the back elemetn
        size--;                         // Decrement size
        bubbleDown(1);                  // Bubble the root down
    };

    /*
    % Type getPriority() const
    % Description: Returns the root priority element, in this case, the minimum
    */
    Type getPriority() const
    {
        return heapArray[1];
    };

    /*
    % bool isEmpty()
    % Description: Returns true if the heapArray is empty
    */
    bool isEmpty() const
    {
        if (size == 0)
            return true;
        else
            return false;
    };

    /*
    % void bubbleUp(int i)
    % Description: Takes the element at a given index and bubbles it up into its correct location
    in a minimum heap
    */
    void bubbleUp(int i)
    {
        // if there is only one element or the child is less than the parent...
        if (size == 1 || heapArray[i / 2] < heapArray[i])
            return;

        // while the parent is less than the child...
        while (heapArray[i] < heapArray[i / 2])
        {
            std::swap(heapArray[i], heapArray[i / 2]); // swap the elemnents
            i = i / 2;                                 // update the index
        }
    };

    /*
    % void bubbleDown(int i)
    % Description: Takes an element at a given index and bubbles it down
    to its correct location in a minimum heap.
    */
    void bubbleDown(int i)
    {
        // if there are 2 elements and the child is less than the parent...
        if (size == 2 && heapArray[i * 2] < heapArray[i])
        {
            std::swap(heapArray[i], heapArray[i * 2]);
            return;
        }

        // if there are no elements, 1 element, or the node is a leaf node
        if (size == 0 || size == 1 || 2 * i > size || 2 * i + 1 > size)
            return;

        // while the left or right children are greater than the parent
        while (heapArray[2 * i] < heapArray[i] || heapArray[2 * i + 1] < heapArray[i])
        {
            if (i > size || i * 2 > size || 2 * i + 1 > size) // check for leaf nodes
                return;

            // if both children are less than the parent...
            if (heapArray[2 * i] < heapArray[i] && heapArray[2 * i + 1] < heapArray[i])
            {
                if (heapArray[2 * i] < heapArray[2 * i + 1]) // check the left child
                {
                    std::swap(heapArray[i], heapArray[2 * i]);
                    i = 2 * i;
                }
                else
                {
                    std::swap(heapArray[i], heapArray[2 * i + 1]); // check the right child
                    i = 2 * i + 1;
                }
            }

            else if (heapArray[2 * i] < heapArray[i]) // if the left child is less thn the parent
            {
                std::swap(heapArray[i], heapArray[2 * i]);
                i = 2 * i;
            }

            else if (heapArray[2 * i + 1] < heapArray[i]) // if the right child is less than the parent
            {
                std::swap(heapArray[i], heapArray[2 * i + 1]);
                i = 2 * i + 1;
            }
            else
                return;
        }
        return;
    };

    /*
    % int getSize()
    % Description: Returns the capacity of the heapArray
    */
    int getSize() const
    {
        return capacity;
    };

private:
    int capacity;
    int size;
    Type *heapArray;
};
int main()
{
    // stock file name, simulation file name, line, and part of the line
    std::string stockFile, simFile, line, partOfLine;
    std::ifstream infile;          // file processing
    std::vector<stockType> stocks; // vector of stocks
    std::vector<bool> stockBought; // vector keeping track of which stocks were bought
    int days;                      // number of days for simulation
    double balance, dblNum;        // account balance, value being converted to double

    // Output Formatting
    std::cout << std::fixed;
    std::cout.precision(2);

    // User prompt for the stock file
    std::cout << "\nStocks file: ";
    std::cin >> stockFile;
    std::cout << std::endl;
    infile.open(stockFile);

    while (getline(infile, line)) // Stock file processing
    {
        std::stringstream ss(line);
        stockType newStock;

        // Parses the string for the stock name and dividend
        getline(ss, partOfLine, ',');
        newStock.name = partOfLine;

        getline(ss, partOfLine);
        dblNum = strtod(partOfLine.c_str(), NULL);
        newStock.dividend = dblNum;

        // Adds stock element to vectors
        stocks.push_back(newStock);
        stockBought.push_back(false);
    }
    infile.close();

    std::cout << "Sim file: ";
    std::cin >> simFile;

    std::cout << "\nAmount of days to simulate: ";
    std::cin >> days;

    std::cout << "\nAmount you wish to transfer into brokerage account: ";
    std::cin >> balance;

    infile.open(simFile);

    int count = 0;    // vector index count
    int dayCount = 0; // count of how many days in simulation

    std::cout << std::endl;

    while (dayCount != days)
    {
        count = 0;
        priorityQ<stockType> heap(10);
        std::cout << "Day " << dayCount + 1 << " Current Balance $ " << balance << std::endl
                  << std::endl;

        getline(infile, line); // prices at the beginning of the day

        std::stringstream ss(line);

        while (getline(ss, partOfLine, ',')) // parses line
        {
            dblNum = strtod(partOfLine.c_str(), NULL);
            stocks[count].price = dblNum;

            heap.insert(stocks[count]); // inserts individuals tocks into the heap
            count++;
        }

        stockType min = heap.getPriority();

        double profit = 0; // reset day's profit variable
        count = 0;         // reset vector index variable

        // Process the minimum heap
        while (!heap.isEmpty() && balance > min.price)
        {
            balance -= min.price; // subtract price from the profit & balance
            profit -= min.price;

            for (int i = 0; i < stocks.size(); i++)
            {
                if (min.price == stocks[i].price) // find the minimum element in the vector
                {
                    stocks[i].timesPurchased++; // increment timesPurchased and set bought to true
                    stockBought[i] = true;
                }
            }

            // delete priority element and output the stock bought
            heap.deletePriority();
            std::cout << "Buying one share of " << min.name << " valued at $ " << min.price << " per share" << std::endl;
            min = heap.getPriority();

            count++;
        }
        getline(infile, line); // second line, prices at the end of the day
        std::stringstream sp(line);

        count = 0;
        while (getline(sp, partOfLine, ','))
        {
            dblNum = strtod(partOfLine.c_str(), NULL);

            if (stockBought[count] == true) // if the stock was bought
            {
                balance += dblNum; // add the price at the end of the day to the balance and profit
                profit += dblNum;
            }
            count++;
        }

        // Output the day's profit
        std::cout << std::endl;
        std::cout << "Profit made today $ " << profit << std::endl
                  << std::endl;

        // Reset the stockBought vector for the next day
        for (int i = 0; i < stockBought.size(); i++)
        {
            stockBought[i] = false;
        }

        dayCount++;
    }

    // Output the balance & amount in dividends
    std::cout << "Balance after " << dayCount << " days $ " << balance << std::endl;

    double sum = 0;

    for (int i = 0; i < stocks.size(); i++) // Dividenc calculation
    {
        sum += stocks[i].dividend * stocks[i].timesPurchased;
    }
    std::cout << "Amount in dividends $ " << sum << std::endl
              << std::endl;

    return 0;
}