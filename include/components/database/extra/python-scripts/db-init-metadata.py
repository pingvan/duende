import pandas as pd
import psycopg2
import json

connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

cursor = connection.cursor()

class Film:
    def __init__(self, title, release_date, description, genres, cover_preview_url, cover_full_size_url):
        self.title = title
        self.release_date = release_date
        self.description = description
        self.genres = genres
        self.cover_preview_url = cover_preview_url
        self.cover_full_size_url = cover_full_size_url

class Actor:
    def __init__(self, full_name):
        self.full_name = full_name

movies_data = pd.read_csv('../metadata/movie_data.csv')
movies_credits = pd.read_csv('../metadata/movie_credits.csv')

movies_credits = movies_credits.rename(columns={'movie_id': 'id'})

merged_data = pd.merge(movies_data, movies_credits, on='id', how='inner').dropna(subset=['release_date'])

films = []
actors_set = set()
actor_to_film = dict()

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

    cover_preview_url = None
    cover_full_size_url = None
    if not pd.isna(tup.cover_preview_url):
        cover_preview_url = tup.cover_preview_url
    if not pd.isna(tup.cover_full_size_url):
        cover_full_size_url = tup.cover_full_size_url

    new_film = Film(tup.original_title, tup.release_date, tup.overview, genres_list, cover_preview_url, cover_full_size_url)
    films.append(new_film)

insert_film = """
INSERT INTO films (title, release_date, description, genres)
VALUES (%s, %s, %s, %s) RETURNING id
"""

insert_film_with_covers = """
INSERT INTO films (title, release_date, description, genres, cover_preview_url, cover_full_size_url) 
VALUES (%s, %s, %s, %s, %s, %s) RETURNING id
"""

insert_actor = """
INSERT INTO actors (full_name) 
VALUES (%s) RETURNING id
"""

insert_film_to_actor = """
INSERT INTO actors_to_film (film_id, actor_id) 
VALUES (%s, %s)
"""

film_id_map = {}
for film in films:
    for i, v in enumerate(film.genres):
        if v == 'science fiction':
            film.genres[i] = 'sci-fi'
    genres_array = "{" + ",".join(film.genres) + "}"

    if film.cover_preview_url is not None and film.cover_full_size_url is not None:
        cursor.execute(insert_film_with_covers, (film.title, film.release_date, film.description, genres_array, film.cover_preview_url, film.cover_full_size_url))
    else:
        cursor.execute(insert_film, (film.title, film.release_date, film.description, genres_array))

    film_id = cursor.fetchone()[0]
    film_id_map[film.title] = film_id

actor_id_map = {}
for actor_name in actors_set:
    cursor.execute(insert_actor, (actor_name,))
    actor_id = cursor.fetchone()[0]
    actor_id_map[actor_name] = actor_id

for movie_id, actor_names in actor_to_film.items():
    film_id = film_id_map[movies_data.loc[movies_data['id'] == movie_id]['original_title'].values[0]]
    for actor_name in actor_names:
        actor_id = actor_id_map[actor_name]
        cursor.execute(insert_film_to_actor, (film_id, actor_id))

connection.commit()
cursor.close()
connection.close()
