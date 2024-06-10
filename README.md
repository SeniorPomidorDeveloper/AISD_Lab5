# Вариант №32

### Предметная область

### Компьютерная сеть.

#### Основные свойства графа

Неориентированный граф. 
Вершины графа хранят следующую информацию: 

* уникальное имя компьютера (строка); 
* номер порта, на котором запущен сетевой сервис (беззнаковое целое число). 
Рёбра графа хранят следующую информацию: 
* перечень портов, по которым разрешена передача трафика (вектор беззнаковых целых чисел); 
* задержка при передаче трафика (беззнаковое целое число). 

Операции, поддерживаемые графом: 
* ##### Обход
  Определение перечня компьютеров, на которых запущен указанный сервис, и которые доступны
  с указанного компьютера c учетом ограничений на передачу данных (поиск в ширину).
* ##### Поиск кратчайшего пути между двумя заданными вершинами
  Поиск кратчайшего пути передачи данных между двумя указанными компьютерами, который
  позволяет с учётом ограничений на передачу данных осуществлять коммуникацию по некоторому
  указанному порту (алгоритм Дейкстры).
* ##### Особая операция
  Разбиение сети на автономно функционирующие подсети, в которых все узлы физически связаны
  между собой, без учёта ограничений на запущенные сервисы и на передачу данных (разбиение на
  связные или сильно связные компоненты).
