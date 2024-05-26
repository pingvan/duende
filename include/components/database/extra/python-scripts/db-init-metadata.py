import pandas as pd
import psycopg2
import json

# Establishing the connection to the database
connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

cursor = connection.cursor()

# Defining classes for film and actor
class Film:
    def __init__(self, title, release_date, description, genres):
        self.title = title
        self.release_date = release_date
        self.description = description
        self.genres = genres

class Actor:
    def __init__(self, full_name):
        self.full_name = full_name

# Reading metadata files
movies_data = pd.read_csv('../metadata/movie_data.csv')
movies_credits = pd.read_csv('../metadata/movie_credits.csv')

# Renaming column for merging
movies_credits = movies_credits.rename(columns={'movie_id': 'id'})

# Merging data on the 'id' column
merged_data = pd.merge(movies_data, movies_credits, on='id', how='inner').dropna(subset=['release_date'])

# Initialize lists and dictionaries
films = []
actors_set = set()
actor_to_film = dict()

# Parsing merged data
for tup in merged_data.itertuples():
    genres_list = []
    actors_list = []
    genres_json = json.loads(tup.genres)
    for en in genres_json:
        genres_list.append(en['name'].lower())
    
    cast_json = json.loads(tup.cast)
    actor_to_film[tup.id] = []
    for en in cast_json:
        actors_set.add(en['name'])
        actors_list.append(en['name'])
        actor_to_film[tup.id].append(en['name'])
    
    new_film = Film(tup.original_title, tup.release_date, tup.overview, genres_list)
    films.append(new_film)

# SQL statements with RETURNING clause
insert_film = """
INSERT INTO films (title, release_date, description, genres) 
VALUES (%s, %s, %s, %s) RETURNING id
"""

insert_actor = """
INSERT INTO actors (full_name) 
VALUES (%s) RETURNING id
"""

insert_film_to_actor = """
INSERT INTO actors_to_film (film_id, actor_id) 
VALUES (%s, %s)
"""

# Insert films and get their generated IDs
film_id_map = {}
for film in films:
    for i, v in enumerate(film.genres):
        if v == 'science fiction':
            film.genres[i] = 'sci-fi'
    genres_array = "{" + ",".join(film.genres) + "}"
    cursor.execute(insert_film, (film.title, film.release_date, film.description, genres_array))
    film_id = cursor.fetchone()[0]
    film_id_map[film.title] = film_id

# Insert actors and get their generated IDs
actor_id_map = {}
for actor_name in actors_set:
    cursor.execute(insert_actor, (actor_name,))
    actor_id = cursor.fetchone()[0]
    actor_id_map[actor_name] = actor_id

# Insert into actors_to_film using generated IDs
for movie_id, actor_names in actor_to_film.items():
    film_id = film_id_map[movies_data.loc[movies_data['id'] == movie_id]['original_title'].values[0]]
    for actor_name in actor_names:
        actor_id = actor_id_map[actor_name]
        cursor.execute(insert_film_to_actor, (film_id, actor_id))

# Commit changes and close the connection
connection.commit()
cursor.close()
connection.close()
