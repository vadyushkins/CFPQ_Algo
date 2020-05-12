# Чем я тут занимаюсь

* Дан помеченный граф.
* Дана контекстно-свободная грамматика.
* Надо научиться проверять наличине пути в графе, удовлетворяющего КС грамматике.
* Граф может изменяться. Грамматика -- нет.

# Как пользоваться

```bash
make
./main graph_file.txt grammar_file.txt queries_file.txt --no-time/--with-time
```

# Можно потестировать
```python
python tester.py
```
Есть такие бенчмарки:
* Соберем "Наихудший граф" добавлением ребер "по-тупому" и "по-умному"
* Разберем "Наихудший граф" удалением ребер "по-тупому" и "по-умному"
* Удалим точку сочленения в "Наихудшем графе"
* Соберем "Наихудший граф" и для каждой пары вершин проверим, что ответы "по-тупому" и "по-умному" совпадают

# TODO
* Добавить неискусственные графы
* Визуализировать результаты
* Добавить "ленивые" алгоритмы
* Прокомментировать код
