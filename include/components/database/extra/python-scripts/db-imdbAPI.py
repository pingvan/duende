import requests
import imdb
import psycopg2

from tqdm import tqdm



ia = imdb.IMDb()

conn = psycopg2.connect("postgres://root:strongpassword@localhost:5432/duende")
cursor = conn.cursor()

def get_movie_cover(movie_name, filmed_year):
    search_results = ia.search_movie(movie_name)

    for movie in search_results:
        if movie.get('year') == filmed_year:
            ia.update(movie)
            cover_url = movie.get_fullsizeURL()
            return cover_url
    

def download_image_as_bytes(image_url):
    try:
        response = requests.get(image_url)

        if response.status_code == 200:
            return response.content
        else:
            print("Failed to download image. Status code:", response.status_code)
            return None
    except Exception as e:
        print("An error occurred:", str(e))
        return None


get_films_query = "SELECT title, release_date FROM films LIMIT 8"
insert_film_cover_query = "UPDATE films SET cover = %s WHERE title = %s"

cursor.execute(get_films_query)
films = cursor.fetchall()

pbar = tqdm(len(films), desc="Downloading covers")

for film in films:
    title = film[0]
    release_date = film[1]

    cover_url = get_movie_cover(title, release_date.year)

    if cover_url:
        image_bytes = download_image_as_bytes(cover_url)
        cursor.execute(insert_film_cover_query, (image_bytes, title))
        print("Cover downloaded for", title)
    else:
        print("No cover found for", title)

    pbar.update(1)    

conn.commit()
conn.close()

        