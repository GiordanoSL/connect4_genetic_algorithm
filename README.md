# Connect4 com OpenGL

Este projeto implementa o clássico jogo **Connect4** utilizando a biblioteca **OpenGL** para renderização gráfica.

## Como Jogar

1. **Interação com o Tabuleiro**  
   Para adicionar uma peça ao tabuleiro, clique com o **botão esquerdo do mouse** na coluna onde deseja inserir a peça. A peça será colocada na primeira linha disponível, de baixo para cima.

2. **Fim de Jogo**  
   Quando o jogo terminar, um retângulo aparecerá acima do tabuleiro, indicando o resultado:
   - **Vermelho** (se o jogador 1 venceu)
   - **Azul** (se o jogador 2 venceu)
   - **Cinza** (se houver empate)

3. **Reiniciar o Jogo**  
   Para reiniciar o tabuleiro e começar uma nova partida, basta clicar com o **botão direito do mouse**.

Divirta-se jogando!
## Pré-requisitos

Antes de compilar e executar o projeto, certifique-se de que seu sistema tem as seguintes dependências instaladas:

- **g++** (com suporte a C++17)
- **OpenGL**
- **GLUT**
- **GLU**

### Comando para compilar usando Makefile

  ```bash
  make all
  ```
### Comando para executar usando Makefile

  ```bash
  make run
  ```
