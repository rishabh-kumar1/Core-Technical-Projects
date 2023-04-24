"""Main py."""
import flask
import index.api
import index
import re
import os
import math

stopwords = []
pageranks = {}
inverted_index = {}


def load_index():
    """Load Index."""
    path = os.path.dirname(os.path.realpath(__file__))
    path = os.path.abspath(os.path.join(path, os.pardir))
    f = open(path + '/stopwords.txt')
    for word in f.read().split():
        stopwords.append(word)
    f = open(path + '/pagerank.out')
    for line in f.read().split():
        split_line = line.split(",")
        pageranks[split_line[0]] = split_line[1]
    f = open(path + '/inverted_index/' + index.app.config["INDEX_PATH"])
    for line in f.readlines():
        split_line = line.split()
        inverted_index[split_line[0]] = []
        for i in range(1, len(split_line)):
            inverted_index[split_line[0]].append(split_line[i])


@index.app.route("/api/v1/")
def get_services():
    """Get Services."""
    context = {
        "hits": "/api/v1/hits/",
        "url": "/api/v1/",
    }
    with index.app.app_context():
        return flask.jsonify(**context)


@index.app.route("/api/v1/hits/")
def get_hits():
    """Get Hits."""
    q = flask.request.args.get("q")
    w = flask.request.args.get("w")
    if not w:
        w = .5
    w = float(w)

    words = q.split()
    output = ""
    for word in words:
        output = output + " " + word
    output = re.sub(r"[^a-zA-Z0-9 ]+", "", output)
    output = output.casefold()
    split_words = output.split()
    temp_final_words = [word for word in split_words if word not in stopwords]

    # key: term value: [term freq in query_list, idf]
    final_words_mapping = {}
    final_words = []

    for word in temp_final_words:
        if inverted_index.get(word) is not None:
            final_words.append(word)

    context = {
        "hits": []
    }

    if len(final_words) != len(temp_final_words):
        with index.app.app_context():
            return flask.jsonify(**context)

    print(final_words)

    for word in final_words:
        if final_words_mapping.get(word) is None:
            final_words_mapping[word] = [0]
        final_words_mapping[word][0] += 1

    # key: doc_id value: document vector
    doc_ids = {}

    # key : doc_id value: norm sqrt
    doc_norm = {}

    final_words = list(set(final_words))

    i = 1
    final_words_mapping[final_words[0]].append(inverted_index[
                                               final_words[0]][0])
    while i < len(inverted_index[final_words[0]]):
        x = float(inverted_index[final_words[0]][i+1])
        y = float(inverted_index[final_words[0]][0])
        doc_ids[inverted_index[final_words[0]][i]] = [x * y]
        # sqrt of norm factor for each doc
        doc_norm[inverted_index[
                 final_words[0]][i]] = math.sqrt(
                                       float(inverted_index[
                                             final_words[0]][i+2]))
        i = i + 3

    for word in final_words[1:]:
        temp_ids = {}
        i = 1
        while i < len(inverted_index[word]):
            x = float(inverted_index[word][i+1])
            y = float(inverted_index[word][0])
            temp_ids[inverted_index[word][i]] = x * y
            x = math.sqrt(float(inverted_index[word][i+2]))
            doc_norm[inverted_index[word][i]] = x
            i = i + 3

        all_ids = []
        for key in doc_ids:
            all_ids.append(key)
        for id in all_ids:
            if id in temp_ids:
                doc_ids[id].append(temp_ids[id])
            else:
                del doc_ids[id]
                del doc_norm[id]

        final_words_mapping[word].append(inverted_index[word][0])

    query_vector = []
    for term in final_words_mapping:
        x = final_words_mapping[term][0]
        y = final_words_mapping[term][1]
        query_vector.append(x * y)
    print(query_vector)

    norm_q = 0
    for term in query_vector:
        norm_q += (float(term)*float(term))
    norm_q = math.sqrt(norm_q)

    final_scores = {}

    for id in doc_ids:
        dot_product = 0.0
        for i in range(len(query_vector)):
            dot_product += float(query_vector[i]) * doc_ids[id][i]
        tf_idf = dot_product/(norm_q * doc_norm[id])
        weighted_score = (w * float(pageranks[id])) + ((1-w) * tf_idf)
        final_scores[id] = weighted_score

    for id in final_scores:
        temp = {}
        temp["docid"] = int(id)
        temp["score"] = final_scores[id]
        context["hits"].append(temp)

    context["hits"] = sorted(context["hits"],
                             key=lambda a: a["score"],
                             reverse=True)

    with index.app.app_context():
        return flask.jsonify(**context)
