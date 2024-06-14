<div align="center">
    <img src="client/ui/img/duende_wallpaper.png" alt="Duende Logo" />
    <h1>Duende</h1>
    <p>- приложение, где вы можете пополнить свой список фильмов и друзей.</p>
    <a href="#особая-благодарность">Благодарность</a> •
    <a href="#о-проекте">О проекте</a> •
    <a href="#почему-duende">Особенности</a> •
    <a href="#посмотреть-в-действии">Превью</a> •
    <a href="#как-установить">Установка</a> •
    <a href="#об-авторах">Авторы</a>
    <h2></h2>
</div>

Prefer English? Read this: <a href="./README.md">English version</a>

<div align="center">
    <img src="client/ui/img/profile.gif" alt="My page gif" />
</div>

## Особая благодарность:

Хотелось бы выразить искреннюю благодарность ментору нашего проекта, <a href="https://github.com/Vladislav0Art">Владиславу Артюхову</a>. За время разработки мы открыли для себя много нового не только в рамках поставленных задач, но и за их пределами. Опыт, полученный при работе с Владом, его вклад, ценные рекомендации, регулярные онлайн-встречи для решения возникших на пути проблем, помогли нам превратить идею в жизнь. Его наставничество за кулисами нашего кино-приложения напоминало работу опытного режиссёра, помогающего своей команде достичь наилучшего результата. 

Мы глубоко признательны Владу за его поддержку, профессионализм и стремление к совершенству, благодаря которым Duende стало возможным.

## О проекте:

**Duende** (от исп. - способность произведения искусства затронуть человеческую душу) - приложение, где любители кино могут пообщаться на интересные им темы. 

Пользователь делится своими вкусами в кинематографе, на основе которых мы рекомендуем ему собеседников или фильмы к просмотру. Вы можете отправлять понравившимся людям приглашения на приватную переписку (и даже там мы сгенерируем для вас фильм, который подойдёт обоим!), а также создавать групповые чаты на большие компании. Зайдя в чей-то профиль, можно увидеть не только общую информацию о пользователе, но и то, какие жанры он предпочитает, каких актёров любит и как отзывается о просмотренных фильмах.

Duende - платформа для знакомств и общения, где всегда есть, о чём поговорить!

<div align="center">
    <img src="client/ui/img/logo.png" alt="Duende Logo" width="200" />
</div>

## Почему Duende?

- **Сервис рекомендаций:** пользователи и фильмы - для одного или двоих.
- **Чаты:** приватные и групповые с различными возможностями и привилегиями участников.
- **Взаимодействие между пользователями:** сервисы подписки, блокировки, поиска.
- **Детальные профили:** любимые жанры, актёры, список просмотренных фильмов с выставленными рейтингами, а также информация о себе: био, любимая цитата, фотография.
- **Другие сервисы:** сервисы авторизации и приглашений в приватные и групповые чаты.

## Посмотреть в действии:

Вы можете посмотреть <a href="https://docs.google.com/presentation/d/1Ex-56XNowumhmJBxGufaQl8g7dLR2u_9ogB1gdCyv50/edit#slide=id.p1">презентацию</a> нашего проекта, а также <a href="https://youtu.be/vyidtRRDYiE">демонстрационное видео</a> на YouTube.

<div align="center">
    <div>
        <img src="client/ui/img/recs.gif" alt="Recommendations gif" />
        <p>Рекомендации</p>
        <br>
    </div>
    <div>
        <img src="client/ui/img/groupchat_creation.gif" alt="Group chat creation gif" />
        <p>Создание группового чата</p>
        <br>
    </div>
    <div>
        <img src="client/ui/img/movie_recs.gif" alt="Movie recs gif" />
        <p>Рекомендации фильмов в чате</p>
        <br>
    </div>
    <div>
        <img src="client/ui/img/subs.gif" alt="Subs gif" />
        <p>Подписки</p>
        <br>
    </div>
</div>


## Как установить?

<details>

<summary><strong>Настройка gRPC для сервера и клиента</strong></summary>

---

Для настройки как сервера, так и клиента, необходимо сначала установить gRPC. Вы можете использовать [gRPC C++ Quickstart Guide](https://grpc.io/docs/languages/cpp/quickstart/) или сделать это по инструкции ниже:

```bash
# Настройка каталога установки gRPC
export MY_INSTALL_DIR=$HOME/.local
mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"

# Установка CMake и зависимостей gRPC
sudo apt-get update   
sudo apt-get install -y cmake
sudo apt-get install -y build-essential autoconf libtool pkg-config
sudo apt-get install -y git

# Установка OpenSSL
sudo apt-get install -y openssl
sudo apt-get install -y libssl-dev

# Установка gRPC
git config --global http.postBuffer 524288000 # Изменение размера буфера для избежания проблем с клонированием
git clone --recurse-submodules -b v1.64.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
cd grpc
mkdir -p cmake/build
cd cmake/build/
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make -j 4
sudo make install
```

</details>

<details>

<summary><strong>Сборка сервера</strong></summary>

---

```bash
# Установка GoogleTest
git clone https://github.com/google/googletest.git -b v1.14.0
cd googletest/
mkdir build
cd build
cmake ..
make
sudo make install

# Установка Libpq
sudo apt-get install -y libpq-dev

# Клонирование репозитория Duende
git clone https://github.com/pingvan/duende.git
git submodule update --init --recursive
cd duende/
mkdir -p build
cd build/
cmake ..
make
cd ../certs
bash generate_certs.sh
cd ..
bash export_env.sh
```

</details>

<details>

<summary><strong>Сборка клиента</strong></summary>

---

```bash
# Установка Qt-6
sudo apt-get install -y qt6-base-dev

# Внутри директории duende
cd client
mkdir -p build
cd build
cmake ..
```
</details>


## Об авторах:

<p align="center">
  <b>В главных ролях:</b><br>
  Альбов Егор - <a href="https://github.com/pingvan">pingvan</a><br>
  Колесова Соня - <a href="https://github.com/kolesovasonya">kolesovasonya</a><br>
  Чернокульская Алина - <a href="https://github.com/chernaaaa">chernaaaa</a><br>
  <br>
  <b>Под руководством:</b><br>
  Артюхов Владислав - <a href="https://github.com/Vladislav0Art">Vladislav0Art</a><br>
  <br>
  <b>Место и время действия:</b><br>
  НИУ Высшая школа экономики, Санкт-Петербург<br>
  весна 2024
</p>