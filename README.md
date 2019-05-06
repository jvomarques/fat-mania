    === README ===
    
    Desenvolvedor: João Victor Marques de Oliveira/jvomarques@gmail.com
    -------------------------------------------------------------------------------------------------
    Jogo desenvolvido na linguagem C++ e com a biblioteca para criação de jogos Allegro5. 
    
Usando uma perspectiva 2D e a biblioteca para criação de jogos Allegro5, o layout do jogo é visto com uma visão de cima para baixo, da “cabeça” dos personagens. O personagem controlado pelo usuário realiza um movimento contínuo a uma velocidade constante, sendo a direção alterada pelo usuário através dos botões direcionais do teclado (cima, baixo, esquerda, direita). O personagem se movimenta em uma matriz e tem a possibilidade de adquirir itens e poderes durante seu percurso. O objetivo final é chegar do outro lado no tabuleiro (matriz). Para isso, precisa superar obstáculos, em forma de paredes como em um labirinto, e enfrentar inimigos durante o percurso. Os itens e poderes coletados podem ser utilizados contra os inimigos. Caso haja colisão entre o personagem e um inimigo e nenhum vida esteja sob sua posse, o jogo terá seu fim. Caso contrário, uma vida será decrementada do personagem e ele retornará a sua posição inicial no jogo. Ao chegar ao local estabelecido do outro lado do tabuleiro e com nenhum inimigo vivo, o usuário ganhará o jogo.
    
Pilhas, filas e listas encadeadas foram implementadas e utilizadas na aplicação para solução de alguns problemas. A pilha foi usada para armazenar os poderes adquiridos pelo personagem, onde um novo poder sobrepõe a ordem de prioridade de saída do antigo poder, a lista encadeada para guardar os itens adquiridos pelo personagem no percurso em uma mochila e a fila de eventos para armazenar os eventos de teclado computados de acordo com o comportamento da estrutura, onde o primeiro a entrar é o primeiro a sair. Outro problema enfrentado no jogo foi o da inteligência artificial dos inimigos, cuja solução pôde ser implementada através de um algoritmo de autoria própria, que indica o caminho que deve ser seguido pelos personagens controlados pelo computador. 
    
    -------------------------------------------------------------------------------------------------
    COMO COMPILAR:
    - Instalar biblioteca Allegro: https://wiki.allegro.cc/index.php?title=Getting_Started#Installing_Allegro
    - Via terminal:
    1) make fatmania
    2) ./fatmania
    

