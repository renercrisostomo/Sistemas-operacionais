class Queue(object):
    def __init__(self, limit=5):
        self.que = [None] * limit
        self.possicao = int
        self.limit = limit
        self.front = 0
        self.rear = 0
        self.size = 0

    def isEmpty(self):
        return self.size <= 0

    def enQueue(self, item):
        if self.size == self.limit:
            return
        else:
            self.que[int(self.rear)] = item
            self.rear = (self.rear + 1) % self.limit
            self.size += 1

    def deQueue(self):
        if self.size == 0:
            return
        else:
            item = self.que[int(self.front)]
            self.front = (self.front + 1) % self.limit
            self.size -= 1
            return item

    def queueRear(self):
        if self.size > 0:
            return self.que[int(self.rear)]
        else:
            raise IndexError

    def queueFront(self):
        if self.size > 0:
            return self.que[int(self.front)]
        else:
            raise IndexError

    def size(self):
        return self.size

    def getValueAtPosition(self, position):
        if position < 1 or position > self.size + 1:
            raise IndexError
        else:
            return self.que[(self.front + position - 1) % self.limit]
        
    def setValueAtPosition(self, position, value):
        if position < 1 or position > self.size + 1:
            raise IndexError
        else:
            self.que[(self.front + position - 1) % self.limit] = value
            #self.printQueue()

    def printQueue(self):
        if self.isEmpty():
            print("A fila está vazia.")
        else:
            print("Fila:")
            print("Posição\tValor")
            for i in range(self.size):
                print(f"{i + 1}\t{self.que[(self.front + i) % self.limit]}")