import psycopg2

# Connect to the PostgreSQL database
connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

# Create a cursor object to execute SQL queries
cursor = connection.cursor()

# Define SQL statements to create tables

create_genre_type = """DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'genre') THEN
        CREATE TYPE genre AS ENUM ('action', 'adventure', 'comedy', 'drama', 'horror', 'sci-fi', 'fantasy', 'thriller',
    'mystery', 'romance', 'animation', 'documentary',
    'crime', 'western', 'musical', 'historical', 'war',
    'biographical', 'sports', 'family');
    END IF;
END $$
"""

create_invite_status_type = """DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'invite_status') THEN
        CREATE TYPE invite_status AS ENUM ('sent', 'accepted', 'declined');
    END IF;
END $$
"""

create_clients_table = """
CREATE TABLE IF NOT EXISTS clients (
    id SERIAL PRIMARY KEY,
    email VARCHAR(256) NOT NULL,
    login VARCHAR(50) NOT NULL
)
"""

create_tokens_table = """
CREATE TABLE IF NOT EXISTS tokens (
    client_id INT NOT NULL,
    refresh_token text NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(id)
)
"""

create_passwords_table = """
CREATE TABLE IF NOT EXISTS passwords (
    client_id INT NOT NULL,
    password_hash VARCHAR(64) NOT NULL,
    password_salt VARCHAR(10) NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(id)
)
"""

create_forms_table = """
CREATE TABLE IF NOT EXISTS forms (
    id SERIAL PRIMARY KEY,
    photo BYTEA,
    quote VARCHAR(50),
    about VARCHAR(250),
    client_id INT NOT NULL,
    favourite_genres genre[],
    FOREIGN KEY (client_id) REFERENCES clients(id)
)
"""

create_producers_table = """
CREATE TABLE IF NOT EXISTS producers (
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(25),
    last_name VARCHAR(25)
)
"""

create_views_table = """
CREATE TABLE IF NOT EXISTS views (
    id SERIAL PRIMARY KEY,
    ratio INT NOT NULL,
    client_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(id),
    FOREIGN KEY (film_id) REFERENCES films(id)
)
"""

create_films_table = """
CREATE TABLE IF NOT EXISTS films (
    id SERIAL PRIMARY KEY,
    title VARCHAR(25) NOT NULL,
    description VARCHAR(250) NOT NULL,
    genres genre[]
)
"""

create_producers_by_film_table = """
CREATE TABLE IF NOT EXISTS producers_by_film (
    producer_id INT not null,
    film_id INT NOT NULL,
    FOREIGN KEY (producer_id) REFERENCES producers(id),
    FOREIGN KEY (film_id) REFERENCES films(id)
)
"""

create_actors_table = """
CREATE TABLE actors(
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(25),
     last_name VARCHAR(25)
)
"""

create_actors_to_form_table = """
CREATE TABLE actors_to_form(
    form_id INT NOT NULL,
    actor_id INT NOT NULL,
    FOREIGN KEY(form_id) REFERENCES forms(id),
    FOREIGN KEY(actor_id) REFERENCES actors(id)
)
"""

create_actors_to_film_table = """
CREATE TABLE actors_to_film(
    film_id INT NOT NULL,
    actor_id INT NOT NULL,
    FOREIGN KEY(film_id) REFERENCES films(id),
    FOREIGN KEY(actor_id) REFERENCES actors(id)
)
"""

create_chats_table = """
CREATE TABLE chats (
    id SERIAL PRIMARY KEY,
    created_at TIMESTAMP NOT NULL,
    is_group_chat BOOLEAN NOT NULL,
    name VARCHAR(50) NOT NULL,
    -- cover BYTEA,
    description text NOT NULL,
    users_count INT NOT NULL
    --users_ids INT[] NOT NULL,
    --admin_ids INT[] NOT NULL
);
"""

create_messages_table = """
CREATE TABLE message (
    id SERIAL PRIMARY KEY,
    sender_id INT NOT NULL,
    chat_id INT NOT NULL,
    time TIMESTAMP NOT NULL,
    message_text text NOT NULL,
    FOREIGN KEY(sender_id) REFERENCES clients(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);
"""

create_chats_memberships_table = """
CREATE TABLE chat_membership (
    id SERIAL PRIMARY KEY,
    client_id INT NOT NULL,
    chat_id INT NOT NULL,
    FOREIGN KEY(client_id) REFERENCES clients(id),
    FOREIGN KEY(chat_id) REFERENCES chats(id)
);
"""

create_invites_table = """
CREATE TABLE invites (
    id SERIAL PRIMARY KEY,
    invite_text VARCHAR(300),
    invite_status invite_status,
    invite_from_client INT NOT NULL,
    invite_to_client INT NOT NULL,
    FOREIGN KEY(invite_from_client) REFERENCES clients(id),
    FOREIGN KEY(invite_to_client) REFERENCES clients(id)
)
"""

# Execute SQL statements to create tables
cursor.execute(create_genre_type)
cursor.execute(create_invite_status_type)
cursor.execute(create_clients_table)
cursor.execute(create_tokens_table)
cursor.execute(create_passwords_table)
cursor.execute(create_forms_table)
cursor.execute(create_producers_table)
cursor.execute(create_films_table)
cursor.execute(create_views_table)
cursor.execute(create_producers_by_film_table)
cursor.execute(create_actors_table)
cursor.execute(create_actors_to_form_table)
cursor.execute(create_actors_to_film_table)
cursor.execute(create_chats_table)
cursor.execute(create_messages_table)
cursor.execute(create_chats_memberships_table)
cursor.execute(create_invites_table)


# Commit the transaction
connection.commit()

# Close the cursor and connection
cursor.close()
connection.close()
