CREATE TABLE clients (
    id SERIAL PRIMARY KEY,
    email VARCHAR(25) NOT NULL,
    login VARCHAR(10) NOT NULL
);

CREATE TABLE tokens (
    client_id INT NOT NULL,
    auth_token text NOT NULL,
    refresh_token text NOT NULL
);

CREATE TABLE passwords (
    client_id INT NOT NULL,
    password_hash VARCHAR(50) NOT NULL,
    password_salt VARCHAR(10) NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(id)
);

CREATE TYPE genre AS ENUM ('action', 'adventure', 'comedy',
    'drama', 'horror', 'sci-fi', 'fantasy', 'thriller',
    'mystery', 'romance', 'animation', 'documentary',
    'crime', 'western', 'musical', 'historical', 'war',
    'biographical', 'sports', 'family');

CREATE TABLE producers (
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(25),
    last_name VARCHAR(25)
);

CREATE TABLE views (
    id SERIAL PRIMARY KEY,
    ratio INT NOT NULL,
    client_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(id),
    FOREIGN KEY (film_id) REFERENCES films(id)
);

CREATE TABLE forms (
    photo BYTEA,
    quote VARCHAR(50),
    about VARCHAR(250),
    client_id INT NOT NULL,
    favourite_genres genre[],
    views_id INT[],
    FOREIGN KEY (client_id) REFERENCES clients(id)
);

CREATE TABLE films (
    id SERIAL PRIMARY KEY,
    title VARCHAR(25) NOT NULL,
    description VARCHAR(250) NOT NULL,
    genres genre[]
);

CREATE TABLE producers_by_film (
    producer_id INT not null,
    film_id INT NOT NULL,
    FOREIGN KEY (producer_id) REFERENCES producers(id),
    FOREIGN KEY (film_id) REFERENCES films(id)
)