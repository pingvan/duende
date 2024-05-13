SET TIME ZONE 'Europe/Moscow';

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
    photo BYTEA,
    quotation VARCHAR(50), 
    about VARCHAR(250),
    user_id INT NOT NULL,
    favourite_genres genre[],
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS films (
    id SERIAL PRIMARY KEY,
    title text NOT NULL,
    release_date TIMESTAMP NOT NULL,
    description text NOT NULL,
    genres genre[],
    cover_preview_url text,
    cover_full_size_url text
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
    created_at TIMESTAMP default NOW(),
    is_group_chat BOOLEAN NOT NULL,
    chat_name VARCHAR(50),
    cover BYTEA,
    description text,
    users_count INT
);

CREATE TABLE IF NOT EXISTS chat_membership (
    user_id INT NOT NULL,
    chat_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);

CREATE TABLE IF NOT EXISTS chat_admins (
    user_id INT NOT NULL, 
    chat_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);

CREATE TABLE IF NOT EXISTS events (
    id SERIAL PRIMARY KEY,
    event_type event_types NOT NULL,
    user_id INT,
    new_name VARCHAR(50)
    -- event_name VARCHAR(50) NOT NULL --strore json with event data
);

CREATE TABLE IF NOT EXISTS chat_events (
    user_id INT NOT NULL,
    chat_id INT NOT NULL,
    event_id INT NOT NULL,
    time_stamp TIMESTAMP default NOW(),
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id), 
    FOREIGN KEY(event_id) REFERENCES events(id)
);

CREATE TABLE IF NOT EXISTS messages (
    id SERIAL PRIMARY KEY,
    sender_id INT NOT NULL,
    chat_id INT NOT NULL,
    sended_at TIMESTAMP default NOW(),
    message_text_data text NOT NULL,
    FOREIGN KEY(sender_id) REFERENCES users(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);


CREATE TABLE IF NOT EXISTS invites (
    id SERIAL PRIMARY KEY,
    invite_text VARCHAR(300),
    invite_from_user INT NOT NULL,
    invite_to_user INT NOT NULL,
    sended_at TIMESTAMP default NOW(),
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

CREATE TABLE IF NOT EXISTS subscriptions (
    subscription_from INT NOT NULL,
    subscription_to INT NOT NULL,
    is_mutual BOOLEAN DEFAULT FALSE,
    FOREIGN KEY(subscription_from) REFERENCES users(id),
    FOREIGN KEY(subscription_to) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS black_list (
    user_id INT NOT NULL,
    blocked_user_id INT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id),
    FOREIGN KEY(blocked_user_id) REFERENCES users(id)
)