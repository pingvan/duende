CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    email VARCHAR(256) NOT NULL,
    login VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS tokens (
    user_id INT NOT NULL,
    refresh_token text NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS passwords (
    user_id INT NOT NULL,
    password_hash VARCHAR(64) NOT NULL,
    password_salt VARCHAR(10) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS forms (
    id SERIAL PRIMARY KEY,
    -- photo BYTEA,
    quotation VARCHAR(50) NOT NULL, 
    about VARCHAR(250) NOT NULL,
    user_id INT NOT NULL,
    favourite_genres genre[],
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS films (
    id SERIAL PRIMARY KEY,
    title text NOT NULL,
    description text NOT NULL,
    genres genre[]
);

CREATE TABLE IF NOT EXISTS views (
    id SERIAL PRIMARY KEY,
    ratio INT NOT NULL,
    user_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (film_id) REFERENCES films(id)
);

CREATE TABLE IF NOT EXISTS producers(
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(25),
    last_name VARCHAR(25)
);

CREATE TABLE IF NOT EXISTS producers_by_film(
    producer_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY(producer_id) REFERENCES producers(id),
    FOREIGN KEY(film_id) REFERENCES films(id)
);

CREATE TABLE IF NOT EXISTS actors(
    id SERIAL PRIMARY KEY,
    full_name text NOT NULL
);

CREATE TABLE IF NOT EXISTS actors_to_form(
    form_id INT NOT NULL,
    actor_id INT NOT NULL,
    FOREIGN KEY(form_id) REFERENCES forms(id),
    FOREIGN KEY(actor_id) REFERENCES actors(id)
);

CREATE TABLE IF NOT EXISTS actors_to_film(
    film_id INT NOT NULL,
    actor_id INT NOT NULL,
    FOREIGN KEY(film_id) REFERENCES films(id),
    FOREIGN KEY(actor_id) REFERENCES actors(id)
);

--https://stackoverflow.com/questions/8416017/is-name-a-special-keyword-in-postgresql

CREATE TABLE IF NOT EXISTS chats (
    id SERIAL PRIMARY KEY,
    created_at TIMESTAMP NOT NULL,
    is_group_chat BOOLEAN NOT NULL,
    chat_name VARCHAR(50) NOT NULL,
    -- cover BYTEA,
    description text NOT NULL,
    users_count INT NOT NULL
    --users_ids INT[] NOT NULL,
    --admin_ids INT[] NOT NULL
);

CREATE TABLE IF NOT EXISTS chat_membership (
    user_id INT NOT NULL,
    chat_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);

CREATE TABLE IF NOT EXISTS messages (
    id SERIAL PRIMARY KEY,
    sender_id INT NOT NULL,
    chat_id INT NOT NULL,
    sended_at TIMESTAMP NOT NULL,
    message_text_data text NOT NULL,
    FOREIGN KEY(sender_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);

CREATE TABLE IF NOT EXISTS messages (
    id SERIAL PRIMARY KEY,
    sender_id INT NOT NULL,
    chat_id INT NOT NULL,
    sended_at TIMESTAMP NOT NULL,
    message_text_data text NOT NULL,
    FOREIGN KEY(sender_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);


CREATE TABLE IF NOT EXISTS invites (
    id SERIAL PRIMARY KEY,
    invite_text VARCHAR(300),
    invite_status invite_status NOT NULL,
    invite_from_user INT NOT NULL,
    invite_to_user INT NOT NULL,
    FOREIGN KEY(invite_from_user) REFERENCES users(id),
    FOREIGN KEY(invite_to_user) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS unhandled_invites (
    user_id INT NOT NULL,
    invite_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(invite_id) REFERENCES invites(id)
);

CREATE TABLE IF NOT EXISTS unhandled_messages (
    user_id INT NOT NULL,
    message_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(message_id) REFERENCES messages(id)
);