CREATE TABLE users(
    id SERIAL PRIMARY KEY,
    email VARCHAR(256) NOT NULL,
    login VARCHAR(50) NOT NULL
);

CREATE TABLE tokens(
    user_id INT NOT NULL,
    refresh_token text NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id)
);

CREATE TABLE passwords(
    user_id INT NOT NULL,
    password_hash VARCHAR(64) NOT NULL,
    password_salt VARCHAR(10) NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id)
);

CREATE TABLE forms(
    id SERIAL PRIMARY KEY,
    photo BYTEA,
    quote VARCHAR(50),
    about VARCHAR(250),
    user_id INT NOT NULL,
    favourite_genres genre[],
    FOREIGN KEY(user_id) REFERENCES users(id)
);

CREATE TYPE genre AS ENUM(
    'action',
    'adventure',
    'comedy',
    'drama',
    'horror',
    'sci-fi',
    'fantasy',
    'thriller',
    'mystery',
    'romance',
    'animation',
    'documentary',
    'crime',
    'western',
    'musical',
    'historical',
    'war',
    'biographical',
    'sports',
    'family'
);

CREATE TABLE views(
    id SERIAL PRIMARY KEY,
    ratio INT NOT NULL,
    user_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(film_id) REFERENCES films(id)
);

CREATE TABLE films(
    id SERIAL PRIMARY KEY,
    title VARCHAR(25) NOT NULL,
    cover BYTEA,
    description VARCHAR(250) NOT NULL,
    genres genre[]
);

CREATE TABLE producers(id SERIAL PRIMARY KEY, first_name VARCHAR(25), last_name VARCHAR(25));

CREATE TABLE producers_by_film(
    producer_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY(producer_id) REFERENCES producers(id),
    FOREIGN KEY(film_id) REFERENCES films(id)
);

CREATE TABLE actors(
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(25),
    last_name VARCHAR(25),
    photo BYTEA
);

CREATE TABLE actors_to_form(
    form_id INT NOT NULL,
    actor_id INT NOT NULL,
    FOREIGN KEY(form_id) REFERENCES forms(id),
    FOREIGN KEY(actor_id) REFERENCES actors(id)
);

CREATE TABLE actors_to_film(
    film_id INT NOT NULL,
    actor_id INT NOT NULL,
    FOREIGN KEY(film_id) REFERENCES films(id),
    FOREIGN KEY(actor_id) REFERENCES actors(id)
);

--https://stackoverflow.com/questions/8416017/is-name-a-special-keyword-in-postgresql

CREATE TABLE chats (
    id SERIAL PRIMARY KEY,
    created_at TIMESTAMP NOT NULL,
    is_group_chat BOOLEAN NOT NULL,
    name VARCHAR(50) NOT NULL,
    cover BYTEA,
    description text NOT NULL,
    users_count INT NOT NULL
    --users_ids INT[] NOT NULL,
    --admin_ids INT[] NOT NULL
);

CREATE TABLE messages (
    id SERIAL PRIMARY KEY,
    sender_id INT NOT NULL,
    chat_id INT NOT NULL,
    time TIMESTAMP NOT NULL,
    message_text text NOT NULL,
    FOREIGN KEY(sender_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);

CREATE TABLE chat_membership (
    id SERIAL PRIMARY KEY,
    user_id INT NOT NULL,
    chat_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);

CREATE TYPE invite_status AS ENUM ('sent', 'accepted', 'declined');

CREATE TABLE invites (
    id SERIAL PRIMARY KEY,
    invite_text VARCHAR(300),
    invite_status invite_status,
    invite_from_user INT NOT NULL,
    invite_to_user INT NOT NULL,
    FOREIGN KEY(invite_from_user) REFERENCES users(id),
    FOREIGN KEY(invite_to_user) REFERENCES users(id)
);

--everything updated in scripts

CREATE TABLE unhandled_messages (
    user_id INT NOT NULL,
    message_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(message_id) REFERENCES messages(id)
);

CREATE TABLE unhandled_invites (
    user_id INT NOT NULL,
    invite_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(invite_id) REFERENCES invites(id)
);