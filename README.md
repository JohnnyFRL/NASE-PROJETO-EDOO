O projeto consiste no desenvolvimento de um Sistema de Agendamento Acadêmico, inspirado no funcionamento do NASE (Núcleo de Apoio à Saúde do Estudante da UFPE).
A proposta é criar uma aplicação em C++ utilizando Programação Orientada a Objetos (POO) para gerenciar o fluxo de atendimento de estudantes, desde o cadastro até o histórico de atendimentos .
O sistema busca resolver problemas como:
Desorganização de atendimentos
Falta de priorização de casos urgentes
Dificuldade no controle de informações dos alunos

🎯 Objetivo do Projeto
Desenvolver um sistema que permita:
Cadastro e gerenciamento de alunos
Realização de triagens com níveis de prioridade
Agendamento de atendimentos
Controle de fila de espera inteligente
Registro de histórico de atendimentos

⚙️ O que a equipe pretende implementar
Com base no backlog e na proposta do sistema, pretendemos implementar:
🧩 1. Gestão de Usuários
Cadastro de alunos (CRUD)
Marcação de aluno como “não validado”
Visualização de dados do aluno
🧠 2. Triagem
Avaliação do aluno por um profissional
Definição de prioridade: alta, média ou baixa
Validação do aluno após triagem
📅 3. Agendamento
Solicitação de agendamento
Associação entre aluno e profissional
Restrição: aluno precisa estar validado
⏳ 4. Fila de Espera Inteligente
Inserção automática quando não há vagas
Ordenação por prioridade
Preenchimento automático de vagas
📊 5. Histórico
Registro dos atendimentos realizados
Consulta de histórico por aluno

🏗️ Modelagem do Sistema (Ideia inicial)
O sistema será estruturado com as seguintes classes:
Pessoa (classe base)
nome, idade, id
Aluno (herda de Pessoa)
status (validado ou não)
Profissional (herda de Pessoa)
Triagem
prioridade
observações
Agendamento
data, horário, aluno, profissional
Sistema
controle geral

🧠 Ideia de Diagrama (UML simplificado)
Pessoa
├── Aluno
└── Profissional

Aluno ─── Triagem
Aluno ─── Agendamento ─── Profissional
Aluno ─── Histórico

⚙️ Regras de Negócio
Aluno não pode agendar sem triagem
Fila organizada por prioridade
Cancelamentos controlados
Registro obrigatório de atendimentos

💾 Tecnologias
Linguagem: C++
Paradigma: Programação Orientada a Objetos
Persistência: Arquivos (.txt ou .json)
CRUD completo

🎯 Objetivo Final
Criar um sistema funcional que simule um ambiente real de atendimento acadêmico, aplicando na prática conceitos como:
Encapsulamento
Herança
Polimorfismo
Estruturas de dados (fila/prioridade)

