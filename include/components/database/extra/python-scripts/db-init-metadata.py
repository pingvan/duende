import pandas as pd
import psycopg2
import json

connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

cursor = connection.cursor()

class film:
    def __init__(self, id, title, description, genres):
        self.id = id
        self.title = title
        self.description = description
        self.genres = genres

class actor:
    def __init__(self, first_name, last_name):
        self.first_name = first_name
        self.last_name = last_name


movies_data = pd.read_csv('../metadata/movie_data.csv')
movies_credits = pd.read_csv('../metadata/movie_credits.csv')

movies_credits = movies_credits.rename(columns={'movie_id' : 'id'})


merged_data = pd.merge(movies_data, movies_credits, on='id', how='inner')

# merged_data.to_csv('created.csv', index=False)

films = []
actor_to_film = dict()
actors_to_id = dict()

for tup in merged_data.itertuples():
    genreee = []
    actorss = []
    genres_json = json.loads(tup.genres)
    for en in genres_json:
        genreee.append(en['name'].lower())
    cast_json = json.loads(tup.cast)
    actor_to_film[tup.id] = []
    for en in cast_json:
        actorss.append(en['name'])
        actor_to_film[tup.id].append(en['id'])
        actors_to_id[en['id']] = en['name']
    new_film = film(tup.id, tup.original_title, tup.overview, genreee)
    films.append(new_film)

insert_film = """
INSERT INTO films (id, title, description, genres) VALUES (%s, %s, %s, %s)
"""

insert_actors="""
INSERT INTO actors (id, full_name) VALUES (%s, %s)
"""

insert_film_to_actor="""
INSERT INTO actors_to_film (film_id, actor_id) VALUES (%s,  %s)
"""

for film in films:
    for i, v in enumerate(film.genres):
        if v == 'science fiction':
            film.genres[i] = 'sci-fi'    
    genres_array = "{" + ",".join(film.genres) + "}"
    cursor.execute(insert_film, (film.id, film.title, film.description, genres_array))
    
for id, name in actors_to_id.items():
    cursor.execute(insert_actors, (id, name))


for film_id, actors_id in actor_to_film.items():
    for actor_id in actors_id:
        cursor.execute(insert_film_to_actor, (film_id, actor_id))


connection.commit()
cursor.close()
connection.close()
