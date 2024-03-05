import psycopg2

# Connect to the PostgreSQL database
connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

# Create a cursor object to execute SQL queries
cursor = connection.cursor()

# Define SQL statements to create tables

create_genre_type = """
CREATE TYPE genre AS ENUM ('action', 'adventure', 'comedy',
    'drama', 'horror', 'sci-fi', 'fantasy', 'thriller',
    'mystery', 'romance', 'animation', 'documentary',
    'crime', 'western', 'musical', 'historical', 'war',
    'biographical', 'sports', 'family')
"""

create_clients_table = """
CREATE TABLE IF NOT EXISTS clients (
    id SERIAL PRIMARY KEY,
    email VARCHAR(25) NOT NULL,
    login VARCHAR(10) NOT NULL
)
"""

create_tokens_table = """
CREATE TABLE IF NOT EXISTS tokens (
    client_id INT NOT NULL,
    auth_token text NOT NULL,
    refresh_token text NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(id)
)
"""

create_passwords_table = """
CREATE TABLE IF NOT EXISTS passwords (
    client_id INT NOT NULL,
    password_hash VARCHAR(50) NOT NULL,
    password_salt VARCHAR(10) NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(id)
)
"""

create_forms_table = """
CREATE TABLE IF NOT EXISTS forms (
    photo BYTEA,
    quote VARCHAR(50),
    about VARCHAR(250),
    client_id INT NOT NULL,
    favourite_genres genre[],
    views_id INT[],
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

# Execute SQL statements to create tables
cursor.execute(create_genre_type)
cursor.execute(create_clients_table)
cursor.execute(create_tokens_table)
cursor.execute(create_passwords_table)
cursor.execute(create_forms_table)
cursor.execute(create_producers_table)
cursor.execute(create_films_table)
cursor.execute(create_views_table)
cursor.execute(create_producers_by_film_table)

# Commit the transaction
connection.commit()

# Close the cursor and connection
cursor.close()
connection.close()
