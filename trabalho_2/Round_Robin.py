from fila import Queue

class Round_Robin:
    def __init__(self, quantum):
        self.quantum = quantum
        self.terminals = None
        self.tempo = 0
        self.tempo_terminado = None
        self.tempo_total = 0
        self.qde_processos = 0
        self.q = None
        self.q2 = None
    
    def ler_entrada(self):
        self.q = Queue(int(input("Quantos processos serão executados: ")))
        self.q2 = Queue(self.q.limit)
        self.qde_processos = self.q.limit
        self.terminals = [None] * self.qde_processos
        self.tempo_terminado = [None] * self.qde_processos
        for i in range(self.qde_processos):
            self.terminals[i] = 0
            tempo_inicial = int(input(f"Digite o tempo de execução do processo P{i + 1}: "))
            self.q.enQueue(tempo_inicial)
            self.q2.enQueue(tempo_inicial)
            self.tempo_total += tempo_inicial
    
    def roundrobin(self):
        self.ler_entrada()
        self.quantum = int(input("Digite o quantum: "))
        aux = 0
        for self.tempo in range(self.tempo_total):
            self.tempo = aux
            for i in range(self.qde_processos):
                if (self.terminals[i] == 0):
                    print(f"[\t{self.tempo} \t|\t P{i + 1} \t|\t {self.q2.getValueAtPosition(i + 1)} \t]")
                    if (self.q2.getValueAtPosition(i + 1) > self.quantum):
                        self.q2.setValueAtPosition(i + 1, self.q2.getValueAtPosition(i + 1) - self.quantum)
                        self.tempo += self.quantum
                    else:
                        self.tempo += self.q2.getValueAtPosition(i + 1)
                        self.q2.setValueAtPosition(i + 1, self.q2.getValueAtPosition(i + 1) - self.q2.getValueAtPosition(i + 1))
                        print(f"[\t{self.tempo} \t|\t P{i + 1} \t|\t {self.q2.getValueAtPosition(i + 1)} \t]")
                    if (self.q2.getValueAtPosition(i + 1) == 0):
                        self.terminals[i] = 1
                    aux = self.tempo
                    self.tempo_terminado[i] = self.tempo
        print(f"\nTempo total: {self.tempo_total}")
        self.imprimir()
        
    def imprimir(self):
        print("Processo\t\t Tempo de execução\t Tempo de chegada\t Tempo de espera\t Tempo de termino")
        for i in range(self.qde_processos):
            print(f"P{i + 1}\t\t\t {self.q.getValueAtPosition(i + 1)}\t\t\t 0\t\t\t {self.tempo_terminado[i] - self.q.getValueAtPosition(i + 1)}\t\t\t {self.tempo_terminado[i]}")
            
        print(f"Tempo médio de espera: {(sum(self.tempo_terminado) - sum(self.q.que)) / self.qde_processos}\nTempo médio de termino: {sum(self.tempo_terminado) / self.qde_processos}\nVazão: {self.qde_processos / self.tempo_total}")
        
rr = Round_Robin(5)
rr.roundrobin()
