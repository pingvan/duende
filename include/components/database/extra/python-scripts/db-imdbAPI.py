import requests
import imdb

ia = imdb.IMDb()

def get_movie_cover(movie_name):
    search_results = ia.search_movie(movie_name)

    print(search_results)

    if search_results:
        movie = search_results[0]

        ia.update(movie)

        print(movie.keys())
        if 'cover url' in movie.keys():
            return movie['cover url']
        else:
            return "Cover image not found for {}".format(movie_name)
    else:
        return "Movie not found: {}".format(movie_name)
    

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



movie_name = "Titanic"
cover_url = get_movie_cover(movie_name)
print("Cover URL for '{}': {}".format(movie_name, cover_url))


image_url = cover_url
image_bytes = download_image_as_bytes(image_url)

if image_bytes:
    print("Image downloaded as bytes successfully!")
    # print(image_bytes)
else:
    print("Failed to download image as bytes.")