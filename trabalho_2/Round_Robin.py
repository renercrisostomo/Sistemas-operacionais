import numpy as np
import matplotlib.pyplot as plt

from fila import Queue

class Round_Robin:
    def __init__(self):
        self.quantum = None
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
    
    def roundrobin(self, valor):
        if self.q is None:
            self.ler_entrada()
        self.quantum = valor
        aux = 0
        processos_concluidos = 0
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
                        processos_concluidos += 1
                    aux = self.tempo
                    self.tempo_terminado[i] = self.tempo
        print(f"\nTempo total: {self.tempo_total}")
        return self.imprimir(processos_concluidos)
        
    def imprimir(self, processos_concluidos):
        print("Processo\t\t Tempo de execução\t Tempo de chegada\t Tempo de espera\t Tempo de termino")
        for i in range(self.qde_processos):
            print(f"P{i + 1}\t\t\t {self.q.getValueAtPosition(i + 1)}\t\t\t 0\t\t\t {self.tempo_terminado[i] - self.q.getValueAtPosition(i + 1)}\t\t\t {self.tempo_terminado[i]}")
        
        tempo_medio_espera = (sum(self.tempo_terminado) - sum(self.q.que)) / self.qde_processos
        tempo_medio_termino = sum(self.tempo_terminado) / self.qde_processos
        vazao = processos_concluidos / self.tempo_total
            
        print(f"Tempo médio de espera: {tempo_medio_espera}\nTempo médio de termino: {tempo_medio_termino}\nVazão: {vazao}")
        return tempo_medio_espera, tempo_medio_termino, vazao
    
quantum = np.arange(30)
tempoMedioEsperaArray, tempoMedioRetornoArray, vazaoArray = np.zeros(30), np.zeros(30), np.zeros(30)
rr = Round_Robin()
rr.ler_entrada()
for i in range(30):
    tempoMedioEsperaArray[i], tempoMedioRetornoArray[i], vazaoArray[i] = rr.roundrobin(i+1)

plt.plot(quantum, tempoMedioEsperaArray, label='Tempo médio de espera')
plt.plot(quantum, tempoMedioRetornoArray, label='Tempo médio de retorno')
plt.plot(quantum, vazaoArray, label='Vazão')
plt.xlabel('Quantum')
plt.ylabel('Valores')
plt.legend()
plt.show()
print(f"Tempo médio de espera: {tempoMedioEsperaArray}\nTempo médio de termino: {tempoMedioRetornoArray}\nVazão: {vazaoArray}")
